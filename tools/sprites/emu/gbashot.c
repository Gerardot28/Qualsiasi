/* gbashot: tiny headless mGBA driver for scripted screenshots.
 * Usage: gbashot ROM SCRIPT [SAVEFILE]
 * Script commands (one per line, '#' comments):
 *   wait N              run N frames with no keys
 *   press KEYS [N] [M]  hold KEYS (e.g. A, B, START, A+B, UP) for N frames (default 4),
 *                       then release for M frames (default 12)
 *   hold KEYS N         hold KEYS for N frames (no release wait)
 *   repeat C KEYS [N] [M]  do 'press' C times
 *   shot FILE.ppm       write the current frame (240x160) as PPM
 *   savestate FILE      write a save state
 *   loadstate FILE      load a save state
 *   rtc MS              freeze the real-time clock at MS milliseconds since the Unix epoch
 * Environment: GBASHOT_RTC=MS freezes the clock from power-on (default: 2026-01-01 12:00:01 UTC,
 * so day/night tinting is deterministic); GBASHOT_RTC=host keeps the host clock.
 */
#include <mgba/core/core.h>
#include <mgba/core/serialize.h>
#include <mgba-util/vfs.h>
#include <mgba/core/log.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void noLog(struct mLogger* l, int c, enum mLogLevel lv, const char* f, va_list a) { (void) l; (void) c; (void) lv; (void) f; (void) a; }
static struct mLogger nullLogger = { .log = noLog };

static struct mCore* core;
static color_t* vbuf;
static unsigned W, H;

static uint32_t parseKeys(const char* s) {
	uint32_t k = 0;
	char buf[128]; strncpy(buf, s, sizeof(buf) - 1); buf[sizeof(buf) - 1] = 0;
	for (char* t = strtok(buf, "+"); t; t = strtok(NULL, "+")) {
		if (!strcmp(t, "A")) k |= 1 << 0;
		else if (!strcmp(t, "B")) k |= 1 << 1;
		else if (!strcmp(t, "SELECT")) k |= 1 << 2;
		else if (!strcmp(t, "START")) k |= 1 << 3;
		else if (!strcmp(t, "RIGHT")) k |= 1 << 4;
		else if (!strcmp(t, "LEFT")) k |= 1 << 5;
		else if (!strcmp(t, "UP")) k |= 1 << 6;
		else if (!strcmp(t, "DOWN")) k |= 1 << 7;
		else if (!strcmp(t, "R")) k |= 1 << 8;
		else if (!strcmp(t, "L")) k |= 1 << 9;
		else if (!strcmp(t, "NONE")) k |= 0;
		else fprintf(stderr, "unknown key %s\n", t);
	}
	return k;
}

static void run(int n, uint32_t keys) {
	core->setKeys(core, keys);
	for (int i = 0; i < n; ++i) core->runFrame(core);
}

static void shot(const char* path) {
	FILE* f = fopen(path, "wb");
	if (!f) { perror(path); return; }
	fprintf(f, "P6\n%u %u\n255\n", W, H);
	for (unsigned y = 0; y < H; ++y) {
		for (unsigned x = 0; x < W; ++x) {
			color_t c = vbuf[y * W + x];
			unsigned char px[3];
#ifdef COLOR_16_BIT
			px[0] = ((c >> 11) & 0x1F) << 3; px[1] = ((c >> 5) & 0x3F) << 2; px[2] = (c & 0x1F) << 3;
#else
			px[0] = c & 0xFF; px[1] = (c >> 8) & 0xFF; px[2] = (c >> 16) & 0xFF;
#endif
			fwrite(px, 1, 3, f);
		}
	}
	fclose(f);
}

int main(int argc, char** argv) {
	if (argc < 3) { fprintf(stderr, "usage: %s ROM SCRIPT [SAV]\n", argv[0]); return 1; }
	mLogSetDefaultLogger(&nullLogger);
	core = mCoreFind(argv[1]);
	if (!core || !core->init(core)) { fprintf(stderr, "no core\n"); return 1; }
	mCoreInitConfig(core, NULL);
	core->desiredVideoDimensions(core, &W, &H);
	vbuf = calloc(W * H, sizeof(color_t));
	core->setVideoBuffer(core, vbuf, W);
	if (!mCoreLoadFile(core, argv[1])) { fprintf(stderr, "load failed\n"); return 1; }
	if (argc > 3) {
		struct VFile* sv = VFileOpen(argv[3], O_CREAT | O_RDWR);
		if (sv) core->loadSave(core, sv);
	}
	const char* rtcEnv = getenv("GBASHOT_RTC");
	if (!rtcEnv || strcmp(rtcEnv, "host")) {
		core->rtc.override = RTC_FIXED;
		core->rtc.value = rtcEnv ? strtoll(rtcEnv, NULL, 10) : 1767268801000LL;
	}
	core->reset(core);
	FILE* sc = !strcmp(argv[2], "-") ? stdin : fopen(argv[2], "r");
	if (!sc) { perror(argv[2]); return 1; }
	char line[512];
	int lineNo = 0;
	while (fgets(line, sizeof(line), sc)) {
		++lineNo;
		char* hash = strchr(line, '#'); if (hash) *hash = 0;
		char cmd[64] = {0}, a1[256] = {0}, a2[64] = {0}, a3[64] = {0}, a4[64] = {0};
		int n = sscanf(line, "%63s %255s %63s %63s %63s", cmd, a1, a2, a3, a4);
		if (n <= 0) continue;
		if (!strcmp(cmd, "wait")) run(atoi(a1), 0);
		else if (!strcmp(cmd, "press")) { run(n > 2 ? atoi(a2) : 4, parseKeys(a1)); run(n > 3 ? atoi(a3) : 12, 0); }
		else if (!strcmp(cmd, "hold")) run(atoi(a2), parseKeys(a1));
		else if (!strcmp(cmd, "repeat")) {
			int c = atoi(a1); uint32_t k = parseKeys(a2);
			for (int i = 0; i < c; ++i) { run(n > 3 ? atoi(a3) : 4, k); run(n > 4 ? atoi(a4) : 12, 0); }
		}
		else if (!strcmp(cmd, "shot")) shot(a1);
		else if (!strcmp(cmd, "rtc")) { core->rtc.override = RTC_FIXED; core->rtc.value = strtoll(a1, NULL, 10); }
		else if (!strcmp(cmd, "savestate")) {
			struct VFile* vf = VFileOpen(a1, O_CREAT | O_TRUNC | O_RDWR);
			if (vf) { mCoreSaveStateNamed(core, vf, SAVESTATE_SAVEDATA | SAVESTATE_RTC); vf->close(vf); }
		}
		else if (!strcmp(cmd, "loadstate")) {
			struct VFile* vf = VFileOpen(a1, O_RDONLY);
			if (vf) { mCoreLoadStateNamed(core, vf, SAVESTATE_SAVEDATA | SAVESTATE_RTC); vf->close(vf); }
			else fprintf(stderr, "line %d: cannot open %s\n", lineNo, a1);
		}
		else fprintf(stderr, "line %d: unknown command %s\n", lineNo, cmd);
	}
	core->deinit(core);
	return 0;
}
