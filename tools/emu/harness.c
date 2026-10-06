/*
 * emu-harness: deterministic headless GBA test harness built on libmgba.
 *
 * Boots a ROM, executes an input script (see README.md for the language),
 * writes PNG screenshots, savestates, memory reads and crash/reset/stuck
 * events.  Runs as fast as the host allows: no window, no audio, no frame
 * limiter.  Determinism: no BIOS (mGBA HLE BIOS), fixed fake RTC epoch that
 * advances with emulated time, TZ forced to UTC, no config files read.
 *
 * Build: tools/emu/setup.sh  (links against libmgba 0.10.x + libpng)
 *
 * Output on stdout is line based and machine readable:
 *   READ   frame=N expr=E addr=0x.. size=B value=0x.. dec=D [label=L]
 *   SHOT   frame=N file=PATH
 *   STATE  save|load frame=N file=PATH
 *   EXPECT ok|FAIL frame=N ...
 *   UNTIL  ok|TIMEOUT frame=N waited=K ...
 *   EVENT  crash|reset|stuck|savedata frame=N pc=0x.. msg="..."
 *   LOG    frame=N level=L cat=C msg="..."   (game errors, de-duplicated)
 *   ECHO   text
 *   DONE   frames=N ... exit=X
 */
#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <mgba/flags.h>
#include <mgba/core/blip_buf.h>
#include <mgba/core/core.h>
#include <mgba/core/config.h>
#include <mgba/core/log.h>
#include <mgba/core/serialize.h>
#include <mgba/core/timing.h>
#include <mgba/core/version.h>
#include <mgba/internal/arm/arm.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/gba/cart/gpio.h>
#include <mgba/internal/gba/overrides.h>
#include <mgba/internal/gba/savedata.h>
#include <mgba/internal/gba/video.h>
#include <mgba-util/vfs.h>

#include <png.h>

#define HARNESS_VERSION "1.0"

/* ------------------------------------------------------------------ */
/* globals                                                             */

static struct mCore* core;
static struct GBA* gba;
static struct ARMCore* cpu;
static color_t* vbuf;
static unsigned vw = 240, vh = 160;

static const char* outdir = ".";
static int defaultScale = 1;
static uint64_t frame;          /* frames executed by this harness run */
static uint32_t heldKeys;       /* keys held with keydown */
static uint32_t tapKeys;        /* autotap: tapped on every frame run */
static long tapPeriod = 20, tapHold = 5;
static uint64_t tapPhase;
static int exitCode;

/* options */
static bool optTraceCrash;
static bool optStopOnCrash;
static bool optAllowReset;
static bool optQuiet;
static int optStuckFrames = 600;
static int optLogLimit = 3;     /* identical LOG lines printed at most N times */
static bool haveCounter;
static uint32_t counterAddr;    /* u32 that must never decrease (reset detection) */
static uint32_t lastCounter;
static bool counterValid;

/* event counters */
static int nCrash, nReset, nStuck, nExpectFail, nUntilTimeout, nSavedata;
static bool stopRequested;

/* stuck detection */
static bool haltThisFrame;
static uint64_t lastHaltFrame;
static uint32_t pcMin = UINT32_MAX, pcMax;
static bool stuckReported;

/* PC trace ring (for --trace-crash) */
#define TRACE_LEN 64
static struct { uint64_t frame; uint32_t pc; uint8_t mode; uint8_t thumb; int swi; } traceRing[TRACE_LEN];
static unsigned traceHead;

static void (*origSwi16)(struct ARMCore*, int);
static void (*origSwi32)(struct ARMCore*, int);
static void (*origIllegal)(struct ARMCore*, uint32_t);
static void (*origStub)(struct ARMCore*, uint32_t);

/* symbols */
struct Symbol { char* name; uint32_t addr; };
static struct Symbol* syms;
static size_t nSyms, capSyms;

/* ------------------------------------------------------------------ */
/* small utils                                                         */

static void die(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "emu-harness: ");
	vfprintf(stderr, fmt, ap);
	fputc('\n', stderr);
	va_end(ap);
	exit(1);
}

static void out(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	putchar('\n');
	fflush(stdout);
}

static uint32_t curPC(void) {
	int len = cpu->executionMode == MODE_THUMB ? 2 : 4;
	return (uint32_t) cpu->gprs[ARM_PC] - 2 * len;
}

static void mkdirs(const char* path) {
	char buf[4096];
	snprintf(buf, sizeof(buf), "%s", path);
	for (char* p = buf + 1; *p; ++p) {
		if (*p == '/') {
			*p = 0;
			mkdir(buf, 0755);
			*p = '/';
		}
	}
	mkdir(buf, 0755);
}

static void outPath(char* dst, size_t n, const char* name, const char* ext) {
	/* NAME with a '/' is taken as a path (relative to cwd); otherwise OUTDIR/NAME.ext */
	if (strchr(name, '/')) {
		snprintf(dst, n, "%s", name);
	} else {
		size_t l = strlen(name), el = strlen(ext);
		if (l > el && strcmp(name + l - el, ext) == 0) {
			snprintf(dst, n, "%s/%s", outdir, name);
		} else {
			snprintf(dst, n, "%s/%s%s", outdir, name, ext);
		}
	}
}

static int symCmp(const void* a, const void* b) {
	return strcmp(((const struct Symbol*) a)->name, ((const struct Symbol*) b)->name);
}

static bool parseU32(const char* s, uint32_t* v) {
	if (!s || !*s) {
		return false;
	}
	char* end;
	errno = 0;
	unsigned long long x;
	if (s[0] == '$') {
		x = strtoull(s + 1, &end, 16);
	} else {
		x = strtoull(s, &end, 0);
	}
	if (errno || *end || x > 0xFFFFFFFFULL) {
		/* allow negative small numbers */
		long long y = strtoll(s, &end, 0);
		if (*end || y < INT32_MIN || y > UINT32_MAX) {
			return false;
		}
		x = (uint32_t) y;
	}
	*v = (uint32_t) x;
	return true;
}

static bool isIdent(const char* s) {
	if (!(isalpha((unsigned char) s[0]) || s[0] == '_')) {
		return false;
	}
	for (; *s; ++s) {
		if (!(isalnum((unsigned char) *s) || *s == '_' || *s == '.')) {
			return false;
		}
	}
	return true;
}

static bool parseHexBare(const char* s, uint32_t* v) {
	char tmp[64];
	if (strlen(s) > 40) {
		return false;
	}
	snprintf(tmp, sizeof(tmp), "%s%s", strncasecmp(s, "0x", 2) ? "0x" : "", s);
	return parseU32(tmp, v);
}

/* accepted line formats:  "NAME 0xADDR" | "0xADDR NAME" | nm: "0802a1b4 T NAME" */
static void loadSymbols(const char* path) {
	FILE* f = fopen(path, "r");
	if (!f) {
		die("cannot open symbols file %s: %s", path, strerror(errno));
	}
	char line[1024];
	while (fgets(line, sizeof(line), f)) {
		char a[512], b[512], c[512];
		int n = sscanf(line, "%511s %511s %511s", a, b, c);
		uint32_t addr;
		const char* name = NULL;
		if (n == 3 && parseHexBare(a, &addr) && isIdent(c)) {
			name = c;
		} else if (n == 2 && isIdent(a) && parseU32(b, &addr)) {
			name = a;
		} else if (n == 2 && isIdent(b) && parseHexBare(a, &addr)) {
			name = b;
		}
		if (!name) {
			continue;
		}
		if (nSyms == capSyms) {
			capSyms = capSyms ? capSyms * 2 : 4096;
			syms = realloc(syms, capSyms * sizeof(*syms));
		}
		syms[nSyms].name = strdup(name);
		syms[nSyms].addr = addr;
		++nSyms;
	}
	fclose(f);
	qsort(syms, nSyms, sizeof(*syms), symCmp);
}

static bool lookupSymbol(const char* name, uint32_t* addr) {
	struct Symbol key = { (char*) name, 0 };
	struct Symbol* s = bsearch(&key, syms, nSyms, sizeof(*syms), symCmp);
	if (!s) {
		return false;
	}
	*addr = s->addr;
	return true;
}

/* ------------------------------------------------------------------ */
/* address expressions:  expr := term (('+'|'-') term)*                  */
/*                       term := NUMBER | SYMBOL | '[' expr ']' | '(' expr ')' */
/* '[x]' dereferences a 32-bit word at x (evaluated at run time).       */

struct ExprCtx { const char* p; bool dry; const char* err; };

static uint32_t readMem(uint32_t addr, int size) {
	switch (size) {
	case 1: return core->rawRead8(core, addr, -1) & 0xFF;
	case 2: return core->rawRead16(core, addr & ~1u, -1) & 0xFFFF;
	default: return core->rawRead32(core, addr & ~3u, -1);
	}
}

static void writeMem(uint32_t addr, int size, uint32_t v) {
	switch (size) {
	case 1: core->rawWrite8(core, addr, -1, v); break;
	case 2: core->rawWrite16(core, addr & ~1u, -1, v); break;
	default: core->rawWrite32(core, addr & ~3u, -1, v); break;
	}
}

static uint32_t exprSum(struct ExprCtx* c);

static void skipWs(struct ExprCtx* c) {
	while (*c->p == ' ' || *c->p == '\t') {
		++c->p;
	}
}

static uint32_t exprTerm(struct ExprCtx* c) {
	skipWs(c);
	if (*c->p == '[' || *c->p == '(') {
		char close = *c->p == '[' ? ']' : ')';
		bool deref = *c->p == '[';
		++c->p;
		uint32_t v = exprSum(c);
		skipWs(c);
		if (*c->p != close) {
			c->err = "unbalanced bracket";
			return 0;
		}
		++c->p;
		if (deref && !c->dry) {
			v = readMem(v, 4);
		}
		return v;
	}
	char tok[256];
	size_t n = 0;
	while ((isalnum((unsigned char) *c->p) || *c->p == '_' || *c->p == '$' || *c->p == '.') && n < sizeof(tok) - 1) {
		tok[n++] = *c->p++;
	}
	tok[n] = 0;
	if (!n) {
		c->err = "expected number or symbol";
		return 0;
	}
	uint32_t v;
	if (isdigit((unsigned char) tok[0]) || tok[0] == '$') {
		if (!parseU32(tok, &v)) {
			c->err = "bad number";
			return 0;
		}
		return v;
	}
	if (!lookupSymbol(tok, &v)) {
		static char msg[300];
		snprintf(msg, sizeof(msg), "unknown symbol '%s' (pass --symbols)", tok);
		c->err = msg;
		return 0;
	}
	return v;
}

static uint32_t exprSum(struct ExprCtx* c) {
	uint32_t v = exprTerm(c);
	while (!c->err) {
		skipWs(c);
		if (*c->p == '+') {
			++c->p;
			v += exprTerm(c);
		} else if (*c->p == '-') {
			++c->p;
			v -= exprTerm(c);
		} else {
			break;
		}
	}
	return v;
}

static bool evalExpr(const char* s, bool dry, uint32_t* v, const char** err) {
	struct ExprCtx c = { s, dry, NULL };
	*v = exprSum(&c);
	skipWs(&c);
	if (!c.err && *c.p) {
		c.err = "trailing characters in expression";
	}
	if (err) {
		*err = c.err;
	}
	return !c.err;
}

/* ------------------------------------------------------------------ */
/* events, logging, hooks                                              */

static void writeShot(const char* path, int scale);
static bool saveStateTo(const char* path);

static void dumpTrace(void) {
	if (!optTraceCrash) {
		return;
	}
	out("TRACE last %d samples (oldest first):", TRACE_LEN);
	for (unsigned i = 0; i < TRACE_LEN; ++i) {
		unsigned k = (traceHead + i) % TRACE_LEN;
		if (!traceRing[k].pc && !traceRing[k].frame) {
			continue;
		}
		if (traceRing[k].swi >= 0) {
			out("TRACE frame=%" PRIu64 " pc=0x%08X mode=0x%02X %s swi=0x%02X", traceRing[k].frame, traceRing[k].pc,
			    traceRing[k].mode, traceRing[k].thumb ? "thumb" : "arm", traceRing[k].swi);
		} else {
			out("TRACE frame=%" PRIu64 " pc=0x%08X mode=0x%02X %s", traceRing[k].frame, traceRing[k].pc,
			    traceRing[k].mode, traceRing[k].thumb ? "thumb" : "arm");
		}
	}
	out("TRACE regs r0=%08X r1=%08X r2=%08X r3=%08X r4=%08X r5=%08X r6=%08X r7=%08X", cpu->gprs[0], cpu->gprs[1],
	    cpu->gprs[2], cpu->gprs[3], cpu->gprs[4], cpu->gprs[5], cpu->gprs[6], cpu->gprs[7]);
	out("TRACE regs r8=%08X r9=%08X r10=%08X r11=%08X r12=%08X sp=%08X lr=%08X pc=%08X cpsr=%08X", cpu->gprs[8],
	    cpu->gprs[9], cpu->gprs[10], cpu->gprs[11], cpu->gprs[12], cpu->gprs[13], cpu->gprs[14], cpu->gprs[15],
	    cpu->cpsr.packed);
}

static void traceAdd(int swi) {
	traceRing[traceHead].frame = frame;
	traceRing[traceHead].pc = curPC();
	traceRing[traceHead].mode = cpu->privilegeMode;
	traceRing[traceHead].thumb = cpu->executionMode == MODE_THUMB;
	traceRing[traceHead].swi = swi;
	traceHead = (traceHead + 1) % TRACE_LEN;
}

static void crashArtifacts(const char* kind) {
	if (!optTraceCrash) {
		return;
	}
	char path[4096], name[128];
	snprintf(name, sizeof(name), "%s_f%" PRIu64, kind, frame);
	outPath(path, sizeof(path), name, ".png");
	writeShot(path, 1);
	out("SHOT frame=%" PRIu64 " file=%s", frame, path);
	outPath(path, sizeof(path), name, ".state");
	if (saveStateTo(path)) {
		out("STATE save frame=%" PRIu64 " file=%s", frame, path);
	}
}

static void event(const char* kind, uint32_t pc, const char* fmt, ...) {
	char msg[512];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(msg, sizeof(msg), fmt, ap);
	va_end(ap);
	out("EVENT %s frame=%" PRIu64 " pc=0x%08X msg=\"%s\"", kind, frame, pc, msg);
	bool fatal = false;
	if (!strcmp(kind, "crash")) {
		++nCrash;
		fatal = true;
	} else if (!strcmp(kind, "stuck")) {
		++nStuck;
		fatal = true;
	} else if (!strcmp(kind, "reset")) {
		++nReset;
		fatal = !optAllowReset;
	}
	if (fatal) {
		dumpTrace();
		if (optStopOnCrash) {
			stopRequested = true;
		}
	}
}

/* de-duplicated log of interesting mGBA messages.  fatal/error/warn are
 * streamed as LOG lines; "game errors" (the game poking unused registers,
 * DMA from address 0, ...) are frequent in retail games too, so by default
 * they are only summarised at the end (LOGSUM) unless --log-game-errors. */
#define MAX_LOGKEYS 256
static struct { char* key; char* first; const char* level; const char* cat; uint64_t frame; int count; } logKeys[MAX_LOGKEYS];
static int nLogKeys;
static bool inLogger;
static bool optLogGame;

static void harnessLog(struct mLogger* logger, int category, enum mLogLevel level, const char* format, va_list args) {
	(void) logger;
	if (!(level & (mLOG_FATAL | mLOG_ERROR | mLOG_WARN | mLOG_GAME_ERROR))) {
		return;
	}
	if (inLogger) {
		return;
	}
	inLogger = true;
	char msg[512];
	vsnprintf(msg, sizeof(msg), format, args);
	for (char* p = msg; *p; ++p) {
		if (*p == '"' || *p == '\n') {
			*p = '\'';
		}
	}
	const char* cat = mLogCategoryName(category);
	if (!cat) {
		cat = "?";
	}
	if (cpu && !strncmp(msg, "Jumped to invalid address", 25)) {
		event("crash", curPC(), "%s", msg);
		crashArtifacts("crash");
	}
	/* count/de-dup by category + format string (i.e. ignoring the numbers) */
	char key[600];
	snprintf(key, sizeof(key), "%s:%s", cat, format);
	const char* lv = level == mLOG_FATAL ? "fatal" : level == mLOG_ERROR ? "error" : level == mLOG_WARN ? "warn" : "game_error";
	int idx = -1;
	for (int i = 0; i < nLogKeys; ++i) {
		if (!strcmp(logKeys[i].key, key)) {
			idx = i;
			break;
		}
	}
	if (idx < 0 && nLogKeys < MAX_LOGKEYS) {
		idx = nLogKeys++;
		logKeys[idx].key = strdup(key);
		logKeys[idx].first = strdup(msg);
		logKeys[idx].level = lv;
		logKeys[idx].cat = cat;
		logKeys[idx].frame = frame;
		logKeys[idx].count = 0;
	}
	int count = idx >= 0 ? ++logKeys[idx].count : optLogLimit + 1;
	bool stream = level != mLOG_GAME_ERROR || optLogGame;
	if (stream && !optQuiet && count <= optLogLimit) {
		out("LOG frame=%" PRIu64 " level=%s cat=\"%s\" msg=\"%s\"%s", frame, lv, cat, msg,
		    count == optLogLimit ? " (further identical messages suppressed)" : "");
	}
	inLogger = false;
}

static void logSummary(void) {
	for (int i = 0; i < nLogKeys; ++i) {
		out("LOGSUM count=%d first_frame=%" PRIu64 " level=%s cat=\"%s\" msg=\"%s\"", logKeys[i].count,
		    logKeys[i].frame, logKeys[i].level, logKeys[i].cat, logKeys[i].first);
	}
}

static struct mLogger logger = { .log = harnessLog, .filter = NULL };

static void noteSwi(int imm) {
	if (imm == 0x02 || imm == 0x03 || imm == 0x04 || imm == 0x05) {
		haltThisFrame = true;
	}
	if (optTraceCrash) {
		traceAdd(imm);
	}
	if (imm == 0x00) {
		event("reset", curPC(), "SoftReset SWI 0x00 (lr=0x%08X)", (uint32_t) cpu->gprs[ARM_LR]);
	} else if (imm == 0x26) {
		event("reset", curPC(), "HardReset SWI 0x26 (lr=0x%08X)", (uint32_t) cpu->gprs[ARM_LR]);
	}
}

static void hookSwi16(struct ARMCore* c, int imm) {
	noteSwi(imm & 0xFF);
	origSwi16(c, imm);
}

static void hookSwi32(struct ARMCore* c, int imm) {
	noteSwi((imm >> 16) & 0xFF);
	origSwi32(c, imm);
}

static void hookIllegal(struct ARMCore* c, uint32_t opcode) {
	if (!(c->executionMode == MODE_THUMB && (opcode & 0xFFC0) == 0xE800)) {
		event("crash", curPC(), "undefined/illegal %s opcode 0x%08X", c->executionMode == MODE_THUMB ? "thumb" : "arm",
		      opcode);
		crashArtifacts("crash");
	}
	origIllegal(c, opcode);
}

static void hookStub(struct ARMCore* c, uint32_t opcode) {
	event("crash", curPC(), "unimplemented (stub) opcode 0x%08X", opcode);
	origStub(c, opcode);
}

static void cbSavedata(void* ctx) {
	(void) ctx;
	++nSavedata;
	out("EVENT savedata frame=%" PRIu64 " pc=0x%08X msg=\"cartridge save memory written\"", frame, curPC());
}

/* ------------------------------------------------------------------ */
/* frame stepping                                                      */

static void sampleCpu(void) {
	if (cpu->halted) {
		haltThisFrame = true;
		return;
	}
	if (cpu->privilegeMode == MODE_IRQ) {
		return;
	}
	uint32_t pc = curPC();
	if (pc < pcMin) {
		pcMin = pc;
	}
	if (pc > pcMax) {
		pcMax = pc;
	}
}

static void runFrame(uint32_t keys) {
	if (stopRequested) {
		return;
	}
	if (tapKeys && ((frame - tapPhase) % (uint64_t) tapPeriod) < (uint64_t) tapHold) {
		keys |= tapKeys;
	}
	core->setKeys(core, keys | heldKeys);
	uint32_t fc = core->frameCounter(core);
	int32_t start = mTimingCurrentTime(core->timing);
	haltThisFrame = false;
	unsigned loops = 0;
	while (core->frameCounter(core) == fc &&
	       (uint32_t) (mTimingCurrentTime(core->timing) - start) < VIDEO_TOTAL_LENGTH + VIDEO_HORIZONTAL_LENGTH) {
		core->runLoop(core);
		sampleCpu();
		if (optTraceCrash && (++loops & 63) == 0) {
			traceAdd(-1);
		}
	}
	blip_clear(core->getAudioChannel(core, 0));
	blip_clear(core->getAudioChannel(core, 1));
	++frame;

	/* stuck detection: no Halt/IntrWait/VBlankIntrWait for a long time while
	 * every non-IRQ PC sample stays inside a tiny window */
	if (haltThisFrame) {
		lastHaltFrame = frame;
		pcMin = UINT32_MAX;
		pcMax = 0;
		stuckReported = false;
	} else if (optStuckFrames > 0 && !stuckReported && frame - lastHaltFrame >= (uint64_t) optStuckFrames) {
		if (pcMin != UINT32_MAX && pcMax - pcMin <= 0x100) {
			stuckReported = true;
			event("stuck", curPC(), "no VBlank wait for %d frames, CPU looping in 0x%08X-0x%08X",
			      (int) (frame - lastHaltFrame), pcMin, pcMax);
			crashArtifacts("stuck");
		} else {
			/* busy but moving: restart the window */
			lastHaltFrame = frame;
			pcMin = UINT32_MAX;
			pcMax = 0;
		}
	}

	/* the game legitimately zeroes its counter during boot, so only watch it
	 * once the console has been on for 2 seconds (gba frame counter is part of
	 * the savestate and restarts on reset) */
	if (haveCounter && core->frameCounter(core) >= 120) {
		uint32_t v = readMem(counterAddr, 4);
		if (counterValid && v < lastCounter && !(lastCounter > 0xFFFFFF00u && v < 0x100)) {
			event("reset", curPC(), "watched counter 0x%08X went %u -> %u (game re-initialised)", counterAddr,
			      lastCounter, v);
		}
		lastCounter = v;
		counterValid = true;
	}
}

static uint64_t regionHash(unsigned x0, unsigned y0, unsigned w, unsigned h) {
	uint64_t hash = 1469598103934665603ULL;
	for (unsigned y = y0; y < y0 + h && y < vh; ++y) {
		for (unsigned x = x0; x < x0 + w && x < vw; ++x) {
			hash ^= (uint64_t) (vbuf[y * vw + x] & 0xFFFFFF);
			hash *= 1099511628211ULL;
		}
	}
	return hash;
}

static void runFrames(int n, uint32_t keys) {
	for (int i = 0; i < n && !stopRequested; ++i) {
		runFrame(keys);
	}
}

/* ------------------------------------------------------------------ */
/* screenshots + states                                                */

static void writeShot(const char* path, int scale) {
	if (scale < 1) {
		scale = 1;
	}
	FILE* f = fopen(path, "wb");
	if (!f) {
		fprintf(stderr, "emu-harness: cannot write %s: %s\n", path, strerror(errno));
		exitCode = exitCode ? exitCode : 1;
		return;
	}
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);
	if (setjmp(png_jmpbuf(png))) {
		png_destroy_write_struct(&png, &info);
		fclose(f);
		fprintf(stderr, "emu-harness: libpng error writing %s\n", path);
		return;
	}
	png_init_io(png, f);
	png_set_IHDR(png, info, vw * scale, vh * scale, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
	             PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_set_compression_level(png, 6);
	png_write_info(png, info);
	png_bytep row = malloc(vw * scale * 3);
	for (unsigned y = 0; y < vh; ++y) {
		const uint8_t* src = (const uint8_t*) &vbuf[y * vw];
		for (unsigned x = 0; x < vw; ++x) {
			for (int s = 0; s < scale; ++s) {
				png_bytep d = &row[(x * scale + s) * 3];
				d[0] = src[x * 4 + 0];
				d[1] = src[x * 4 + 1];
				d[2] = src[x * 4 + 2];
			}
		}
		for (int s = 0; s < scale; ++s) {
			png_write_row(png, row);
		}
	}
	png_write_end(png, NULL);
	png_destroy_write_struct(&png, &info);
	free(row);
	fclose(f);
}

#define STATE_FLAGS (SAVESTATE_SAVEDATA | SAVESTATE_RTC | SAVESTATE_METADATA)

static bool saveStateTo(const char* path) {
	struct VFile* vf = VFileOpen(path, O_CREAT | O_TRUNC | O_RDWR);
	if (!vf) {
		fprintf(stderr, "emu-harness: cannot write state %s\n", path);
		return false;
	}
	bool ok = mCoreSaveStateNamed(core, vf, STATE_FLAGS);
	vf->close(vf);
	return ok;
}

static bool loadStateFrom(const char* path) {
	struct VFile* vf = VFileOpen(path, O_RDONLY);
	if (!vf) {
		fprintf(stderr, "emu-harness: cannot read state %s\n", path);
		return false;
	}
	bool ok = mCoreLoadStateNamed(core, vf, STATE_FLAGS);
	vf->close(vf);
	counterValid = false;
	lastHaltFrame = frame;
	pcMin = UINT32_MAX;
	pcMax = 0;
	stuckReported = false;
	return ok;
}

/* ------------------------------------------------------------------ */
/* script                                                              */

enum Op {
	OP_WAIT, OP_PRESS, OP_HOLD, OP_KEYDOWN, OP_KEYUP, OP_RELEASE, OP_MASH, OP_SHOT, OP_SAVESTATE, OP_LOADSTATE,
	OP_READ, OP_WRITE, OP_EXPECT, OP_UNTIL, OP_UNTILANY, OP_AUTOTAP, OP_WAITSTABLE, OP_LOOP, OP_ENDLOOP, OP_BREAKIF, OP_BREAKIFANY, OP_DUMP, OP_ECHO, OP_RESET, OP_QUIT, OP_REGS
};

struct Cmd {
	enum Op op;
	int lineNo;
	char* line;      /* original text, for messages */
	uint32_t keys;
	long n[3];
	int size;        /* 1/2/4 for memory ops */
	char* expr;      /* address expression */
	char* expr2;     /* value expression */
	char* name;      /* file name / label / echo text */
	bool negate;     /* until/expect: VALUE given as !VALUE (wait while equal) */
};

static const struct { const char* name; int bit; } keyNames[] = {
	{ "A", 0 }, { "B", 1 }, { "SELECT", 2 }, { "START", 3 }, { "RIGHT", 4 }, { "LEFT", 5 }, { "UP", 6 },
	{ "DOWN", 7 }, { "R", 8 }, { "L", 9 },
};

static bool parseKeys(const char* s, uint32_t* keys) {
	*keys = 0;
	char buf[128];
	snprintf(buf, sizeof(buf), "%s", s);
	for (char* tok = strtok(buf, "+,|"); tok; tok = strtok(NULL, "+,|")) {
		bool found = false;
		for (size_t i = 0; i < sizeof(keyNames) / sizeof(*keyNames); ++i) {
			if (!strcasecmp(tok, keyNames[i].name)) {
				*keys |= 1u << keyNames[i].bit;
				found = true;
			}
		}
		if (!found) {
			return false;
		}
	}
	return *keys != 0;
}

static void keysToStr(uint32_t keys, char* buf, size_t n) {
	buf[0] = 0;
	for (size_t i = 0; i < sizeof(keyNames) / sizeof(*keyNames); ++i) {
		if (keys & (1u << keyNames[i].bit)) {
			if (buf[0]) {
				strncat(buf, "+", n - strlen(buf) - 1);
			}
			strncat(buf, keyNames[i].name, n - strlen(buf) - 1);
		}
	}
}

static bool parseLong(const char* s, long* v) {
	if (!s) {
		return false;
	}
	char* end;
	errno = 0;
	*v = strtol(s, &end, 0);
	return !errno && *s && !*end;
}

static int sizeFromSuffix(const char* word, const char* prefix) {
	size_t l = strlen(prefix);
	if (strncasecmp(word, prefix, l)) {
		return 0;
	}
	if (!strcmp(word + l, "8")) {
		return 1;
	}
	if (!strcmp(word + l, "16")) {
		return 2;
	}
	if (!strcmp(word + l, "32")) {
		return 4;
	}
	return 0;
}

static struct Cmd* cmds;
static size_t nCmds;

static int scriptError(const char* file, int lineNo, const char* line, const char* msg) {
	fprintf(stderr, "%s:%d: %s\n    %s\n", file, lineNo, msg, line);
	return 1;
}

/* split by whitespace into at most maxTok tokens; the last token takes the rest of the line */
static int tokenize(char* s, char** tok, int maxTok) {
	int n = 0;
	while (*s && n < maxTok) {
		while (*s == ' ' || *s == '\t') {
			++s;
		}
		if (!*s) {
			break;
		}
		tok[n++] = s;
		if (n == maxTok) {
			break;
		}
		while (*s && *s != ' ' && *s != '\t') {
			++s;
		}
		if (*s) {
			*s++ = 0;
		}
	}
	return n;
}

static int parseScript(const char* path) {
	FILE* f = strcmp(path, "-") ? fopen(path, "r") : stdin;
	if (!f) {
		die("cannot open script %s: %s", path, strerror(errno));
	}
	char raw[4096];
	int lineNo = 0, errors = 0;
	size_t cap = 0;
	while (fgets(raw, sizeof(raw), f)) {
		++lineNo;
		char* nl = strpbrk(raw, "\r\n");
		if (nl) {
			*nl = 0;
		}
		char* hash = strchr(raw, '#');
		/* '#' starts a comment except inside echo text */
		char* s = raw;
		while (*s == ' ' || *s == '\t') {
			++s;
		}
		if (strncasecmp(s, "echo", 4) && hash) {
			*hash = 0;
		}
		size_t L = strlen(s);
		while (L && (s[L - 1] == ' ' || s[L - 1] == '\t')) {
			s[--L] = 0;
		}
		if (!*s) {
			continue;
		}
		char work[4096];
		snprintf(work, sizeof(work), "%s", s);
		char* tok[8] = { 0 };
		struct Cmd c = { 0 };
		c.lineNo = lineNo;
		c.line = strdup(s);
		const char* err = NULL;
		int nt;
		char* verb;
		nt = tokenize(work, tok, 8);
		verb = tok[0];
		int sz;
		uint32_t dummy;

		if (!strcasecmp(verb, "wait")) {
			c.op = OP_WAIT;
			if (nt != 2 || !parseLong(tok[1], &c.n[0]) || c.n[0] < 0) {
				err = "usage: wait FRAMES";
			}
		} else if (!strcasecmp(verb, "press")) {
			c.op = OP_PRESS;
			c.n[0] = 6;
			c.n[1] = 10;
			if (nt < 2 || nt > 4 || !parseKeys(tok[1], &c.keys) || (nt >= 3 && !parseLong(tok[2], &c.n[0])) ||
			    (nt == 4 && !parseLong(tok[3], &c.n[1]))) {
				err = "usage: press KEY[+KEY] [HOLD_FRAMES=6 [AFTER_FRAMES=10]]";
			}
		} else if (!strcasecmp(verb, "hold")) {
			c.op = OP_HOLD;
			if (nt != 3 || !parseKeys(tok[1], &c.keys) || !parseLong(tok[2], &c.n[0])) {
				err = "usage: hold KEY[+KEY] FRAMES";
			}
		} else if (!strcasecmp(verb, "keydown") || !strcasecmp(verb, "keyup")) {
			c.op = tolower((unsigned char) verb[3]) == 'd' ? OP_KEYDOWN : OP_KEYUP;
			if (nt != 2 || !parseKeys(tok[1], &c.keys)) {
				err = "usage: keydown|keyup KEY[+KEY]";
			}
		} else if (!strcasecmp(verb, "release")) {
			c.op = OP_RELEASE;
		} else if (!strcasecmp(verb, "mash")) {
			c.op = OP_MASH;
			c.n[1] = 20;
			c.n[2] = 5;
			if (nt < 3 || nt > 5 || !parseKeys(tok[1], &c.keys) || !parseLong(tok[2], &c.n[0]) ||
			    (nt >= 4 && !parseLong(tok[3], &c.n[1])) || (nt == 5 && !parseLong(tok[4], &c.n[2])) ||
			    c.n[1] < 2 || c.n[2] < 1 || c.n[2] >= c.n[1]) {
				err = "usage: mash KEY TOTAL_FRAMES [PERIOD=20 [HOLD=5]]";
			}
		} else if (!strcasecmp(verb, "shot") || !strcasecmp(verb, "screenshot")) {
			c.op = OP_SHOT;
			c.n[0] = 0;
			if (nt < 2 || nt > 3 || (nt == 3 && (!parseLong(tok[2], &c.n[0]) || c.n[0] < 1 || c.n[0] > 8))) {
				err = "usage: shot NAME [SCALE 1..8]";
			}
			c.name = nt >= 2 ? strdup(tok[1]) : NULL;
		} else if (!strcasecmp(verb, "savestate") || !strcasecmp(verb, "loadstate")) {
			c.op = tolower((unsigned char) verb[0]) == 's' ? OP_SAVESTATE : OP_LOADSTATE;
			if (nt != 2) {
				err = "usage: savestate|loadstate NAME";
			}
			c.name = nt >= 2 ? strdup(tok[1]) : NULL;
		} else if ((sz = sizeFromSuffix(verb, "read"))) {
			c.op = OP_READ;
			c.size = sz;
			/* read8 EXPR [LABEL]  -- EXPR must not contain spaces when a label is given */
			if (nt < 2) {
				err = "usage: read8|read16|read32 ADDR [LABEL]";
			} else {
				c.expr = strdup(tok[1]);
				if (nt >= 3) {
					c.name = strdup(tok[2]);
				}
			}
		} else if ((sz = sizeFromSuffix(verb, "write"))) {
			c.op = OP_WRITE;
			c.size = sz;
			if (nt != 3) {
				err = "usage: write8|write16|write32 ADDR VALUE";
			} else {
				c.expr = strdup(tok[1]);
				c.expr2 = strdup(tok[2]);
			}
		} else if ((sz = sizeFromSuffix(verb, "expect"))) {
			c.op = OP_EXPECT;
			c.size = sz;
			if (nt < 3) {
				err = "usage: expect8|expect16|expect32 ADDR VALUE [LABEL]";
			} else {
				c.expr = strdup(tok[1]);
				c.negate = tok[2][0] == '!';
				c.expr2 = strdup(tok[2] + c.negate);
				if (nt >= 4) {
					c.name = strdup(tok[3]);
				}
			}
		} else if ((sz = sizeFromSuffix(verb, "untilany"))) {
			c.op = OP_UNTILANY;
			c.size = sz;
			/* untilany32 BASE STRIDE COUNT VALUE MAX [LABEL] */
			if (nt < 6 || !parseLong(tok[2], &c.n[1]) || !parseLong(tok[3], &c.n[2]) || !parseLong(tok[5], &c.n[0]) ||
			    c.n[0] < 0 || c.n[1] <= 0 || c.n[2] <= 0 || c.n[2] > 4096) {
				err = "usage: untilany8|16|32 BASE STRIDE COUNT [!]VALUE MAX_FRAMES [LABEL]";
			} else {
				c.expr = strdup(tok[1]);
				c.negate = tok[4][0] == '!';
				c.expr2 = strdup(tok[4] + c.negate);
				if (nt >= 7) {
					c.name = strdup(tok[6]);
				}
			}
		} else if ((sz = sizeFromSuffix(verb, "until"))) {
			c.op = OP_UNTIL;
			c.size = sz;
			if (nt < 4 || !parseLong(tok[3], &c.n[0]) || c.n[0] < 0) {
				err = "usage: until8|until16|until32 ADDR [!]VALUE MAX_FRAMES [LABEL]";
			} else {
				c.expr = strdup(tok[1]);
				c.negate = tok[2][0] == '!';
				c.expr2 = strdup(tok[2] + c.negate);
				if (nt >= 5) {
					c.name = strdup(tok[4]);
				}
			}
		} else if (!strcasecmp(verb, "waitstable")) {
			/* waitstable STABLE_FRAMES MAX_FRAMES [X Y W H] */
			c.op = OP_WAITSTABLE;
			long r[4] = { 0, 0, 240, 160 };
			bool okArgs = (nt == 3 || nt == 7) && parseLong(tok[1], &c.n[0]) && parseLong(tok[2], &c.n[1]) &&
			    c.n[0] >= 1 && c.n[1] >= 0;
			for (int k = 0; okArgs && nt == 7 && k < 4; ++k) {
				okArgs = parseLong(tok[3 + k], &r[k]);
			}
			if (!okArgs || r[0] < 0 || r[1] < 0 || r[2] < 1 || r[3] < 1 || r[0] + r[2] > 240 || r[1] + r[3] > 160) {
				err = "usage: waitstable STABLE_FRAMES MAX_FRAMES [X Y W H]  (region inside 240x160)";
			}
			c.n[2] = (r[0] << 24) | (r[1] << 16) | (r[2] << 8) | r[3];
		} else if (!strcasecmp(verb, "autotap")) {
			c.op = OP_AUTOTAP;
			c.n[0] = 20;
			c.n[1] = 5;
			if (nt >= 2 && !strcasecmp(tok[1], "off")) {
				c.keys = 0;
			} else if (nt < 2 || nt > 4 || !parseKeys(tok[1], &c.keys) || (nt >= 3 && !parseLong(tok[2], &c.n[0])) ||
			           (nt == 4 && !parseLong(tok[3], &c.n[1])) || c.n[0] < 2 || c.n[1] < 1 || c.n[1] >= c.n[0]) {
				err = "usage: autotap KEY [PERIOD=20 [HOLD=5]] | autotap off";
			}
		} else if (!strcasecmp(verb, "loop")) {
			c.op = OP_LOOP;
			if (nt != 2 || !parseLong(tok[1], &c.n[0]) || c.n[0] < 0) {
				err = "usage: loop MAX_ITERATIONS ... endloop   ({n} in names = iteration number)";
			}
		} else if (!strcasecmp(verb, "endloop")) {
			c.op = OP_ENDLOOP;
		} else if ((sz = sizeFromSuffix(verb, "breakifany"))) {
			c.op = OP_BREAKIFANY;
			c.size = sz;
			if (nt < 5 || !parseLong(tok[2], &c.n[1]) || !parseLong(tok[3], &c.n[2]) || c.n[1] <= 0 || c.n[2] <= 0 ||
			    c.n[2] > 4096) {
				err = "usage: breakifany8|16|32 BASE STRIDE COUNT [!]VALUE [LABEL]";
			} else {
				c.expr = strdup(tok[1]);
				c.negate = tok[4][0] == '!';
				c.expr2 = strdup(tok[4] + c.negate);
				if (nt >= 6) {
					c.name = strdup(tok[5]);
				}
			}
		} else if ((sz = sizeFromSuffix(verb, "breakif"))) {
			c.op = OP_BREAKIF;
			c.size = sz;
			if (nt < 3) {
				err = "usage: breakif8|16|32 ADDR [!]VALUE [LABEL]";
			} else {
				c.expr = strdup(tok[1]);
				c.negate = tok[2][0] == '!';
				c.expr2 = strdup(tok[2] + c.negate);
				if (nt >= 4) {
					c.name = strdup(tok[3]);
				}
			}
		} else if (!strcasecmp(verb, "dump")) {
			c.op = OP_DUMP;
			if (nt != 4 || !parseLong(tok[3], &c.n[0]) || c.n[0] <= 0 || c.n[0] > 0x1000000) {
				err = "usage: dump NAME ADDR LENGTH";
			} else {
				c.name = strdup(tok[1]);
				c.expr = strdup(tok[2]);
			}
		} else if (!strcasecmp(verb, "echo")) {
			c.op = OP_ECHO;
			const char* rest = s + 4;
			while (*rest == ' ' || *rest == '\t') {
				++rest;
			}
			c.name = strdup(rest);
		} else if (!strcasecmp(verb, "reset")) {
			c.op = OP_RESET;
		} else if (!strcasecmp(verb, "quit") || !strcasecmp(verb, "exit")) {
			c.op = OP_QUIT;
		} else if (!strcasecmp(verb, "regs")) {
			c.op = OP_REGS;
		} else {
			err = "unknown command";
		}
		if (!err && c.expr && !evalExpr(c.expr, true, &dummy, &err)) {
			/* err set */
		}
		if (!err && c.expr2 && !evalExpr(c.expr2, true, &dummy, &err)) {
			/* err set */
		}
		if (err) {
			errors += scriptError(path, lineNo, s, err);
			continue;
		}
		if (nCmds == cap) {
			cap = cap ? cap * 2 : 128;
			cmds = realloc(cmds, cap * sizeof(*cmds));
		}
		cmds[nCmds++] = c;
	}
	if (f != stdin) {
		fclose(f);
	}
	/* match loop/endloop, check breakif placement */
	size_t stack[64];
	int depth = 0;
	for (size_t i = 0; i < nCmds; ++i) {
		struct Cmd* c = &cmds[i];
		if (c->op == OP_LOOP) {
			if (depth == 64) {
				errors += scriptError(path, c->lineNo, c->line, "loops nested too deeply");
				break;
			}
			stack[depth++] = i;
		} else if (c->op == OP_ENDLOOP) {
			if (!depth) {
				errors += scriptError(path, c->lineNo, c->line, "endloop without loop");
				continue;
			}
			size_t start = stack[--depth];
			cmds[start].n[1] = (long) i;
			c->n[1] = (long) start;
		} else if ((c->op == OP_BREAKIF || c->op == OP_BREAKIFANY) && !depth) {
			errors += scriptError(path, c->lineNo, c->line, "breakif outside of a loop");
		}
	}
	while (depth > 0) {
		struct Cmd* c = &cmds[stack[--depth]];
		errors += scriptError(path, c->lineNo, c->line, "loop without endloop");
	}
	return errors;
}

static uint32_t evalOrDie(const char* e, const struct Cmd* c) {
	uint32_t v;
	const char* err;
	if (!evalExpr(e, false, &v, &err)) {
		fprintf(stderr, "line %d: %s: %s\n", c->lineNo, c->line, err);
		return 0;
	}
	return v;
}

static uint32_t sizeMask(int size) {
	return size == 4 ? 0xFFFFFFFFu : size == 2 ? 0xFFFFu : 0xFFu;
}

static long loopIter[64];
static size_t loopAt[64];
static int loopDepth;

/* "{n}" in a name -> current (innermost) loop iteration, 3 digits */
static const char* expandName(const char* name) {
	static char buf[1024];
	const char* p = strstr(name, "{n}");
	if (!p || !loopDepth) {
		return name;
	}
	snprintf(buf, sizeof(buf), "%.*s%03ld%s", (int) (p - name), name, loopIter[loopDepth - 1], p + 3);
	return buf;
}

static bool condTrue(const struct Cmd* c, int* hit) {
	uint32_t a = evalOrDie(c->expr, c);
	uint32_t want = evalOrDie(c->expr2, c) & sizeMask(c->size);
	long count = c->op == OP_BREAKIFANY ? c->n[2] : 1;
	long stride = c->op == OP_BREAKIFANY ? c->n[1] : 0;
	for (long k = 0; k < count; ++k) {
		uint32_t v = readMem(a + (uint32_t) (k * stride), c->size);
		if ((v == want) != c->negate) {
			if (hit) {
				*hit = (int) k;
			}
			return true;
		}
	}
	return false;
}

static void runScript(void) {
	char path[4096], kbuf[64];
	for (size_t i = 0; i < nCmds && !stopRequested; ++i) {
		struct Cmd* c = &cmds[i];
		switch (c->op) {
		case OP_LOOP:
			if (c->n[0] == 0) {
				i = (size_t) c->n[1];
				break;
			}
			loopAt[loopDepth] = i;
			loopIter[loopDepth] = 1;
			++loopDepth;
			break;
		case OP_ENDLOOP: {
			struct Cmd* l = &cmds[c->n[1]];
			if (loopIter[loopDepth - 1] < l->n[0]) {
				++loopIter[loopDepth - 1];
				i = (size_t) c->n[1];
			} else {
				out("LOOP done frame=%" PRIu64 " iterations=%ld line=%d (max reached)", frame, loopIter[loopDepth - 1],
				    l->lineNo);
				--loopDepth;
			}
			break;
		}
		case OP_BREAKIF:
		case OP_BREAKIFANY:
			if (condTrue(c, NULL)) {
				size_t start = loopAt[loopDepth - 1];
				out("LOOP break frame=%" PRIu64 " iterations=%ld line=%d%s%s", frame, loopIter[loopDepth - 1],
				    c->lineNo, c->name ? " label=" : "", c->name ? c->name : "");
				--loopDepth;
				i = (size_t) cmds[start].n[1];
			}
			break;
		case OP_WAIT:
			runFrames(c->n[0], 0);
			break;
		case OP_PRESS:
			runFrames(c->n[0], c->keys);
			runFrames(c->n[1], 0);
			break;
		case OP_HOLD:
			runFrames(c->n[0], c->keys);
			break;
		case OP_KEYDOWN:
			heldKeys |= c->keys;
			break;
		case OP_KEYUP:
			heldKeys &= ~c->keys;
			break;
		case OP_RELEASE:
			heldKeys = 0;
			break;
		case OP_MASH:
			for (long t = 0; t < c->n[0] && !stopRequested; ++t) {
				runFrame((t % c->n[1]) < c->n[2] ? c->keys : 0);
			}
			break;
		case OP_SHOT:
			outPath(path, sizeof(path), expandName(c->name), ".png");
			writeShot(path, c->n[0] ? (int) c->n[0] : defaultScale);
			out("SHOT frame=%" PRIu64 " file=%s", frame, path);
			break;
		case OP_SAVESTATE:
			outPath(path, sizeof(path), expandName(c->name), ".state");
			if (saveStateTo(path)) {
				out("STATE save frame=%" PRIu64 " file=%s", frame, path);
			} else {
				exitCode = 1;
			}
			break;
		case OP_LOADSTATE:
			outPath(path, sizeof(path), c->name, ".state");
			if (loadStateFrom(path)) {
				out("STATE load frame=%" PRIu64 " file=%s", frame, path);
			} else {
				fprintf(stderr, "emu-harness: loadstate failed: %s\n", path);
				exitCode = 1;
				stopRequested = true;
			}
			break;
		case OP_READ: {
			uint32_t a = evalOrDie(c->expr, c);
			uint32_t v = readMem(a, c->size);
			int32_t sv = c->size == 1 ? (int8_t) v : c->size == 2 ? (int16_t) v : (int32_t) v;
			out("READ frame=%" PRIu64 " expr=%s addr=0x%08X size=%d value=0x%0*X dec=%u sdec=%d%s%s", frame, c->expr, a,
			    c->size * 8, c->size * 2, v, v, sv, c->name ? " label=" : "", c->name ? c->name : "");
			break;
		}
		case OP_WRITE: {
			uint32_t a = evalOrDie(c->expr, c);
			uint32_t v = evalOrDie(c->expr2, c) & sizeMask(c->size);
			writeMem(a, c->size, v);
			out("WRITE frame=%" PRIu64 " addr=0x%08X size=%d value=0x%0*X", frame, a, c->size * 8, c->size * 2, v);
			break;
		}
		case OP_EXPECT: {
			uint32_t a = evalOrDie(c->expr, c);
			uint32_t want = evalOrDie(c->expr2, c) & sizeMask(c->size);
			uint32_t v = readMem(a, c->size);
			bool ok = (v == want) != c->negate;
			if (!ok) {
				++nExpectFail;
			}
			out("EXPECT %s frame=%" PRIu64 " expr=%s addr=0x%08X size=%d value=0x%0*X want=%s0x%0*X%s%s", ok ? "ok" : "FAIL",
			    frame, c->expr, a, c->size * 8, c->size * 2, v, c->negate ? "!" : "", c->size * 2, want,
			    c->name ? " label=" : "", c->name ? c->name : "");
			break;
		}
		case OP_UNTIL:
		case OP_UNTILANY: {
			long waited = 0;
			uint32_t a = 0, v = 0, want = 0;
			int hit = -1;
			bool ok;
			for (;;) {
				a = evalOrDie(c->expr, c);
				want = evalOrDie(c->expr2, c) & sizeMask(c->size);
				if (c->op == OP_UNTIL) {
					v = readMem(a, c->size);
					ok = (v == want) != c->negate;
				} else {
					ok = false;
					for (long k = 0; k < c->n[2] && !ok; ++k) {
						v = readMem(a + (uint32_t) (k * c->n[1]), c->size);
						if ((v == want) != c->negate) {
							ok = true;
							hit = (int) k;
						}
					}
				}
				if (ok || waited >= c->n[0] || stopRequested) {
					break;
				}
				runFrame(0);
				++waited;
			}
			if (!ok) {
				++nUntilTimeout;
			}
			if (c->op == OP_UNTIL) {
				out("UNTIL %s frame=%" PRIu64 " waited=%ld expr=%s addr=0x%08X value=0x%0*X want=%s0x%0*X%s%s",
				    ok ? "ok" : "TIMEOUT", frame, waited, c->expr, a, c->size * 2, v, c->negate ? "!" : "",
				    c->size * 2, want, c->name ? " label=" : "", c->name ? c->name : "");
			} else {
				out("UNTIL %s frame=%" PRIu64 " waited=%ld expr=%s addr=0x%08X index=%d want=%s0x%0*X%s%s",
				    ok ? "ok" : "TIMEOUT", frame, waited, c->expr, a, hit, c->negate ? "!" : "", c->size * 2, want,
				    c->name ? " label=" : "", c->name ? c->name : "");
			}
			break;
		}
		case OP_WAITSTABLE: {
			unsigned rx = (c->n[2] >> 24) & 0xFF, ry = (c->n[2] >> 16) & 0xFF, rw = (c->n[2] >> 8) & 0xFF,
			         rh = c->n[2] & 0xFF;
			/* "stable" = no NEW picture for N frames: a frame counts as stable when
			 * its region hash already occurred in the previous 64 frames, so short
			 * periodic animations (blinking/bobbing text arrow, cursor) are allowed
			 * while printing text, fades and scrolling are not */
			enum { RING = 64 };
			uint64_t ring[RING];
			int nring = 0, pos = 0;
			ring[pos++] = regionHash(rx, ry, rw, rh);
			nring = 1;
			long same = 0, waited = 0;
			while (same < c->n[0] && waited < c->n[1] && !stopRequested) {
				runFrame(0);
				++waited;
				uint64_t h = regionHash(rx, ry, rw, rh);
				bool seen = false;
				for (int k = 0; k < nring && !seen; ++k) {
					seen = ring[k] == h;
				}
				same = seen ? same + 1 : 0;
				ring[pos] = h;
				pos = (pos + 1) % RING;
				if (nring < RING) {
					++nring;
				}
			}
			bool ok = same >= c->n[0];
			if (!ok) {
				++nUntilTimeout;
			}
			out("STABLE %s frame=%" PRIu64 " waited=%ld stable=%ld region=%u,%u,%ux%u", ok ? "ok" : "TIMEOUT", frame,
			    waited, same, rx, ry, rw, rh);
			break;
		}
		case OP_AUTOTAP:
			tapKeys = c->keys;
			tapPeriod = c->n[0];
			tapHold = c->n[1];
			tapPhase = frame;
			break;
		case OP_DUMP: {
			uint32_t a = evalOrDie(c->expr, c);
			outPath(path, sizeof(path), expandName(c->name), ".bin");
			FILE* f = fopen(path, "wb");
			if (!f) {
				fprintf(stderr, "emu-harness: cannot write %s\n", path);
				exitCode = 1;
				break;
			}
			for (long k = 0; k < c->n[0]; ++k) {
				fputc(readMem(a + k, 1), f);
			}
			fclose(f);
			out("DUMP frame=%" PRIu64 " addr=0x%08X len=%ld file=%s", frame, a, c->n[0], path);
			break;
		}
		case OP_ECHO:
			out("ECHO frame=%" PRIu64 " %s", frame, expandName(c->name));
			break;
		case OP_RESET:
			core->reset(core);
			counterValid = false;
			out("RESET frame=%" PRIu64, frame);
			break;
		case OP_QUIT:
			stopRequested = true;
			break;
		case OP_REGS:
			keysToStr(heldKeys, kbuf, sizeof(kbuf));
			out("REGS frame=%" PRIu64 " pc=0x%08X lr=0x%08X sp=0x%08X cpsr=0x%08X halted=%d held=%s", frame, curPC(),
			    (uint32_t) cpu->gprs[ARM_LR], (uint32_t) cpu->gprs[ARM_SP], (uint32_t) cpu->cpsr.packed, cpu->halted,
			    kbuf[0] ? kbuf : "-");
			break;
		}
	}
}

/* ------------------------------------------------------------------ */
/* main                                                                */

static int64_t parseRtc(const char* s) {
	/* epoch seconds, or YYYY-MM-DD[THH:MM[:SS]] interpreted as UTC */
	uint32_t v;
	if (parseU32(s, &v)) {
		return v;
	}
	struct tm tm = { 0 };
	int Y, M, D, h = 0, m = 0, sec = 0;
	int n = sscanf(s, "%d-%d-%d%*1[T ]%d:%d:%d", &Y, &M, &D, &h, &m, &sec);
	if (n < 3) {
		die("bad --rtc value '%s' (use epoch seconds or YYYY-MM-DDTHH:MM:SS)", s);
	}
	tm.tm_year = Y - 1900;
	tm.tm_mon = M - 1;
	tm.tm_mday = D;
	tm.tm_hour = h;
	tm.tm_min = m;
	tm.tm_sec = sec;
	return timegm(&tm);
}

static void usage(FILE* f) {
	fprintf(f,
	        "usage: emu-harness --rom ROM.gba --script FILE|- [--out DIR] [options]\n"
	        "  --sav FILE           cartridge save file (created if missing; Flash128K for Emerald)\n"
	        "  --sav-readonly       load --sav but never write it back\n"
	        "  --load-state FILE    load this savestate before running the script\n"
	        "  --save-state-out F   write a savestate after the script finishes\n"
	        "  --scale N            default screenshot scale (1 = native 240x160)\n"
	        "  --rtc T              RTC start (epoch s or YYYY-MM-DDTHH:MM:SS UTC, default 2025-06-01T10:00:00);\n"
	        "                       the clock then advances with emulated time (deterministic)\n"
	        "  --rtc-wallclock      use the host clock instead (non deterministic)\n"
	        "  --symbols FILE       symbol table ('NAME 0xADDR', 'ADDR NAME' or nm output) for expressions\n"
	        "  --watch-counter EXPR u32 that must never decrease (e.g. gMain+0x20); a decrease = reset event\n"
	        "  --trace-crash        on crash/stuck/reset: dump PC trace + registers, save crash_*.png/.state\n"
	        "  --stop-on-crash      abort the script at the first crash/stuck/reset event\n"
	        "  --allow-reset        soft resets are reported but do not fail the run\n"
	        "  --stuck-frames N     frames without VBlank wait in a tight loop = stuck (default 600, 0=off)\n"
	        "  --savetype T         force save type: auto, flash1m, flash512, sram, eeprom, none\n"
	        "  --force-rtc          force the cartridge RTC chip on\n"
	        "  --idle-opt MODE      mGBA idle loop optimisation: remove (default), detect, ignore\n"
	        "  --bios FILE          use a real GBA BIOS instead of mGBA's HLE BIOS\n"
	        "  --log-limit N        print identical emulator log lines at most N times (default 3)\n"
	        "  --log-game-errors    stream 'game error' log lines too (default: only LOGSUM at the end)\n"
	        "  --quiet              do not print LOG lines\n"
	        "exit status: 0 ok, 1 error, 2 script syntax error, 3 expect/until failed, 4 crash/stuck/reset\n");
}

int main(int argc, char** argv) {
	const char *rom = NULL, *script = NULL, *sav = NULL, *loadState = NULL, *saveStateOut = NULL, *symFile = NULL;
	const char *rtcStr = "2025-06-01T10:00:00", *savetype = "auto", *idleOpt = "remove", *bios = NULL,
	           *watchExpr = NULL;
	bool savReadonly = false, rtcWall = false, forceRtc = false;

	for (int i = 1; i < argc; ++i) {
		const char* a = argv[i];
#define ARG(name) (!strcmp(a, name) && (i + 1 < argc || (die("%s needs a value", name), 0)))
		if (ARG("--rom")) {
			rom = argv[++i];
		} else if (ARG("--script")) {
			script = argv[++i];
		} else if (ARG("--out")) {
			outdir = argv[++i];
		} else if (ARG("--sav")) {
			sav = argv[++i];
		} else if (!strcmp(a, "--sav-readonly")) {
			savReadonly = true;
		} else if (ARG("--load-state")) {
			loadState = argv[++i];
		} else if (ARG("--save-state-out")) {
			saveStateOut = argv[++i];
		} else if (ARG("--scale")) {
			defaultScale = atoi(argv[++i]);
		} else if (ARG("--rtc")) {
			rtcStr = argv[++i];
		} else if (!strcmp(a, "--rtc-wallclock")) {
			rtcWall = true;
		} else if (ARG("--symbols")) {
			symFile = argv[++i];
		} else if (ARG("--watch-counter")) {
			watchExpr = argv[++i];
		} else if (!strcmp(a, "--trace-crash")) {
			optTraceCrash = true;
		} else if (!strcmp(a, "--stop-on-crash")) {
			optStopOnCrash = true;
		} else if (!strcmp(a, "--allow-reset")) {
			optAllowReset = true;
		} else if (ARG("--stuck-frames")) {
			optStuckFrames = atoi(argv[++i]);
		} else if (ARG("--savetype")) {
			savetype = argv[++i];
		} else if (!strcmp(a, "--force-rtc")) {
			forceRtc = true;
		} else if (ARG("--idle-opt")) {
			idleOpt = argv[++i];
		} else if (ARG("--bios")) {
			bios = argv[++i];
		} else if (ARG("--log-limit")) {
			optLogLimit = atoi(argv[++i]);
		} else if (!strcmp(a, "--quiet")) {
			optQuiet = true;
		} else if (!strcmp(a, "--log-game-errors")) {
			optLogGame = true;
		} else if (!strcmp(a, "--version")) {
			printf("emu-harness %s (libmgba %s)\n", HARNESS_VERSION, projectVersion);
			return 0;
		} else if (!strcmp(a, "-h") || !strcmp(a, "--help")) {
			usage(stdout);
			return 0;
		} else {
			usage(stderr);
			die("unknown argument %s", a);
		}
#undef ARG
	}
	if (!rom || !script) {
		usage(stderr);
		return 1;
	}
	if (defaultScale < 1 || defaultScale > 8) {
		die("--scale must be 1..8");
	}

	/* determinism: RTC conversions use localtime() */
	setenv("TZ", "UTC", 1);
	tzset();

	if (symFile) {
		loadSymbols(symFile);
	}
	int perr = parseScript(script);
	if (perr) {
		fprintf(stderr, "emu-harness: %d error(s) in script, nothing executed\n", perr);
		return 2;
	}
	if (watchExpr) {
		const char* err;
		if (!evalExpr(watchExpr, true, &counterAddr, &err)) {
			die("--watch-counter: %s", err);
		}
		haveCounter = true;
	}
	mkdirs(outdir);

	mLogSetDefaultLogger(&logger);

	core = mCoreFind(rom);
	if (!core) {
		die("cannot open ROM or unsupported file: %s", rom);
	}
	if (core->platform(core) != mPLATFORM_GBA) {
		die("not a GBA ROM: %s", rom);
	}
	if (!core->init(core)) {
		die("core init failed");
	}
	mCoreInitConfig(core, NULL);
	mCoreConfigSetValue(&core->config, "idleOptimization", idleOpt);
	mCoreConfigSetIntValue(&core->config, "mute", 1);
	mCoreConfigSetIntValue(&core->config, "frameskip", 0);
	if (bios) {
		mCoreConfigSetValue(&core->config, "gba.bios", bios);
		mCoreConfigSetIntValue(&core->config, "useBios", 1);
		mCoreConfigSetIntValue(&core->config, "skipBios", 1);
	} else {
		mCoreConfigSetIntValue(&core->config, "useBios", 0);
	}
	mCoreLoadForeignConfig(core, &core->config);

	core->desiredVideoDimensions(core, &vw, &vh);
	vbuf = calloc(vw * vh, sizeof(color_t));
	core->setVideoBuffer(core, vbuf, vw);
	core->setAudioBufferSize(core, 2048);

	gba = core->board;
	cpu = core->cpu;
	gba->hardCrash = false;
	origSwi16 = cpu->irqh.swi16;
	origSwi32 = cpu->irqh.swi32;
	origIllegal = cpu->irqh.hitIllegal;
	origStub = cpu->irqh.hitStub;
	cpu->irqh.swi16 = hookSwi16;
	cpu->irqh.swi32 = hookSwi32;
	cpu->irqh.hitIllegal = hookIllegal;
	cpu->irqh.hitStub = hookStub;

	struct mCoreCallbacks cbs = { 0 };
	cbs.savedataUpdated = cbSavedata;
	core->addCoreCallbacks(core, &cbs);

	if (!mCoreLoadFile(core, rom)) {
		die("failed to load ROM %s", rom);
	}
	if (bios) {
		struct VFile* bvf = VFileOpen(bios, O_RDONLY);
		if (!bvf || !core->loadBIOS(core, bvf, 0)) {
			die("failed to load BIOS %s", bios);
		}
	}
	if (sav) {
		if (!mCoreLoadSaveFile(core, sav, savReadonly)) {
			die("cannot open save file %s", sav);
		}
	} else {
		/* blank in-memory cartridge save: in-game saving works for the whole run
		 * (and "EVENT savedata" is reported) but nothing touches the disk */
		struct VFile* mem = VFileMemChunk(NULL, 0);
		if (!mem || !core->loadSave(core, mem)) {
			die("cannot create in-memory save");
		}
	}

	if (rtcWall) {
		core->rtc.override = RTC_NO_OVERRIDE;
	} else {
		core->rtc.override = RTC_FAKE_EPOCH;
		core->rtc.value = parseRtc(rtcStr) * 1000LL;
	}

	core->reset(core);

	/* optional hardware overrides (applied after reset, like mGBA's own override path) */
	if (strcasecmp(savetype, "auto")) {
		static const struct { const char* n; enum SavedataType t; } st[] = {
			{ "flash1m", SAVEDATA_FLASH1M }, { "flash512", SAVEDATA_FLASH512 }, { "sram", SAVEDATA_SRAM },
			{ "eeprom", SAVEDATA_EEPROM }, { "none", SAVEDATA_FORCE_NONE },
		};
		bool found = false;
		for (size_t k = 0; k < sizeof(st) / sizeof(*st); ++k) {
			if (!strcasecmp(savetype, st[k].n)) {
				GBASavedataForceType(&gba->memory.savedata, st[k].t);
				found = true;
			}
		}
		if (!found) {
			die("unknown --savetype %s", savetype);
		}
	}
	if (forceRtc && !(gba->memory.hw.devices & HW_RTC)) {
		GBAHardwareInitRTC(&gba->memory.hw);
		GBASavedataRTCRead(&gba->memory.savedata);
	}

	char title[17] = { 0 }, code[9] = { 0 };
	core->getGameTitle(core, title);
	core->getGameCode(core, code);
	static const char* stNames[] = { "none", "sram", "flash512", "flash1m", "eeprom", "eeprom512", "sram512" };
	int stype = gba->memory.savedata.type;
	out("INFO rom=%s title=\"%s\" code=%s savetype=%s rtc=%s rtc_start=%s idle=%s", rom, title, code,
	    stype >= 0 && stype < (int) (sizeof(stNames) / sizeof(*stNames)) ? stNames[stype] : "auto",
	    (gba->memory.hw.devices & HW_RTC) ? "yes" : "no", rtcWall ? "wallclock" : rtcStr, idleOpt);

	if (loadState) {
		if (!loadStateFrom(loadState)) {
			die("failed to load state %s", loadState);
		}
		out("STATE load frame=%" PRIu64 " file=%s", frame, loadState);
	}

	struct timespec t0, t1;
	clock_gettime(CLOCK_MONOTONIC, &t0);
	runScript();
	clock_gettime(CLOCK_MONOTONIC, &t1);
	double secs = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

	if (saveStateOut) {
		if (saveStateTo(saveStateOut)) {
			out("STATE save frame=%" PRIu64 " file=%s", frame, saveStateOut);
		} else {
			exitCode = 1;
		}
	}

	if (!exitCode) {
		if (nCrash || nStuck || (nReset && !optAllowReset)) {
			exitCode = 4;
		} else if (nExpectFail || nUntilTimeout) {
			exitCode = 3;
		}
	}
	logSummary();
	out("DONE frames=%" PRIu64 " gba_frame=%u seconds=%.2f fps=%.0f crashes=%d stuck=%d resets=%d expect_fail=%d "
	    "until_timeout=%d savedata_writes=%d exit=%d",
	    frame, core->frameCounter(core), secs, secs > 0 ? frame / secs : 0.0, nCrash, nStuck, nReset, nExpectFail,
	    nUntilTimeout, nSavedata, exitCode);

	/* flushes + closes the save file */
	core->unloadROM(core);
	mCoreConfigDeinit(&core->config);
	core->deinit(core);
	free(vbuf);
	return exitCode;
}
