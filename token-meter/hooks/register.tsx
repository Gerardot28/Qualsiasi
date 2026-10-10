import { atom, read, update } from 'claude-code'
import type { EngineInterface, Register, SessionRateLimit, TurnUsage } from 'claude-code'

import type { Limit, Meter, Tokens } from '../types'

const PANE = 'token-meter'
const TITLE = 'Token & limiti'

const zero: Tokens = { input: 0, output: 0, cacheRead: 0, cacheWrite: 0, requests: 0 }
const meter = atom({ plugin: 'token-meter', key: 'meter' } as const, {
  session: zero,
  lastTurn: zero,
  limits: [],
} as Meter)

const add = (t: Tokens, u: TurnUsage): Tokens => ({
  input: t.input + u.input_tokens,
  output: t.output + u.output_tokens,
  cacheRead: t.cacheRead + u.cache_read_input_tokens,
  cacheWrite: t.cacheWrite + u.cache_creation_input_tokens,
  requests: t.requests + 1,
})

export const total = (t: Tokens) => t.input + t.output + t.cacheRead + t.cacheWrite

export const fmt = (n: number) =>
  n >= 1_000_000 ? `${(n / 1_000_000).toFixed(2)}M` : n >= 1_000 ? `${(n / 1_000).toFixed(1)}k` : `${n}`

export const bar = (percent: number, width: number) => {
  const filled = Math.max(0, Math.min(width, Math.round((percent / 100) * width)))

  return '█'.repeat(filled) + '░'.repeat(width - filled)
}

const color = (percent: number) => (percent >= 90 ? 'red' : percent >= 70 ? 'yellow' : 'green')

const LABELS: Record<string, string> = {
  seven_day: 'Settimanale (7 giorni)',
  five_hour: 'Sessione (5 ore)',
  spend_limit: 'Limite di spesa',
}

export const untilReset = (resetsAt: string | undefined, now: number) => {
  const at = resetsAt ? Date.parse(resetsAt) : NaN
  if (Number.isNaN(at)) return ''
  const minutes = Math.max(0, Math.round((at - now) / 60_000))
  const d = Math.floor(minutes / 1440)
  const h = Math.floor((minutes % 1440) / 60)
  const m = minutes % 60

  return d > 0 ? `reset tra ${d}g ${h}h` : h > 0 ? `reset tra ${h}h ${m}m` : `reset tra ${m}m`
}

const toLimits = (list: readonly SessionRateLimit[]): Limit[] =>
  [...list]
    .map(l => ({ kind: l.kind, percentUsed: l.percentUsed, resetsAt: l.resetsAt }))
    .sort((a, b) => (a.kind === 'seven_day' ? -1 : b.kind === 'seven_day' ? 1 : 0))

const statusLine = (m: Meter) => {
  const week = m.limits.find(l => l.kind === 'seven_day')
  const parts = [`⚡ ${fmt(total(m.session))} tok`]
  if (week) parts.push(`sett. ${week.percentUsed}%`)
  if (m.usd !== undefined) parts.push(`$${m.usd.toFixed(2)}`)

  return parts.join(' · ')
}

const refresh = async ($: EngineInterface) => {
  const usage = await $.session.usage()
  const now = await $.clock.now()
  const m = await update($, meter, prev => ({
    ...prev,
    usd: usage.cost?.usd ?? prev.usd,
    contextPercent: usage.context.percent ?? prev.contextPercent,
    contextTokens: usage.context.tokens ?? prev.contextTokens,
    contextWindow: usage.context.window,
    limits: usage.rateLimits.length > 0 ? toLimits(usage.rateLimits) : prev.limits,
    updatedAt: now,
  }))
  $.ui.status(statusLine(m))
}

const show = ($: EngineInterface) => $.ui.open({ id: PANE, title: TITLE })

export const register: Register = on => {
  on('session.start', async ($, e, next) => {
    const started = await next(e)
    await $.command.register({
      name: 'token-meter',
      description: 'Apri la barra laterale con token spesi e utilizzo settimanale',
    })
    await refresh($)
    void show($)

    return started
  })

  on('command.run', { command: 'token-meter' }, async $ => {
    await refresh($)
    const opened = await show($)

    return { text: opened.isPlaced ? 'Token meter aperto.' : 'Token meter pronto (allarga la finestra per vederlo).' }
  })

  // Ogni azione (prompt inviato) riattiva la barra e azzera il conteggio del turno.
  on('prompt.submit', ($, e, next) => {
    void update($, meter, prev => ({ ...prev, lastTurn: zero })).catch(() => undefined)
    void show($).catch(() => undefined)

    return next(e)
  })

  // Ogni richiesta al modello (anche dei subagenti) aggiunge i suoi token, in tempo reale.
  on('turn.step', async function* ($, e, next) {
    const response = yield* next(e)
    const usage = response.usage
    if (usage) {
      const now = await $.clock.now()
      const m = await update($, meter, prev => ({
        ...prev,
        session: add(prev.session, usage),
        lastTurn: add(prev.lastTurn, usage),
        model: e.agentId ? prev.model : usage.model,
        updatedAt: now,
      }))
      $.ui.status(statusLine(m))
    }

    return response
  })

  // Il motore spinge qui contesto, costo e finestre di rate-limit quando cambiano.
  on('session.measure', async ($, e, next) => {
    const now = await $.clock.now()
    const m = await update($, meter, prev => ({
      ...prev,
      usd: e.cost?.usd ?? prev.usd,
      contextPercent: e.context.percent ?? prev.contextPercent,
      contextTokens: e.context.tokens ?? prev.contextTokens,
      contextWindow: e.context.window,
      limits: e.rateLimits.length > 0 ? toLimits(e.rateLimits) : prev.limits,
      updatedAt: now,
    }))
    $.ui.status(statusLine(m))

    return next(e)
  })

  on('ui.render', { component: 'Pane', requestId: PANE }, async ($, e) => {
    const { Box, Text } = $.ui.resolve(e)
    const m = await read($, meter)
    const now = await $.clock.now()
    const width = Math.max(8, Math.min(30, e.props.bodyColumns - 8))
    const week = m.limits.find(l => l.kind === 'seven_day')
    const others = m.limits.filter(l => l.kind !== 'seven_day')

    const row = (label: string, value: string) => (
      <Box key={label} flexDirection="row" justifyContent="space-between">
        <Text dimColor>{label}</Text>
        <Text>{value}</Text>
      </Box>
    )

    const gauge = (l: Limit) => (
      <Box key={l.kind} flexDirection="column" marginTop={1}>
        <Text bold>{LABELS[l.kind] ?? l.kind}</Text>
        <Box flexDirection="row">
          <Text color={color(l.percentUsed)}>{bar(l.percentUsed, width)}</Text>
          <Text bold color={color(l.percentUsed)}>
            {' '}
            {l.percentUsed}%
          </Text>
        </Box>
        {l.resetsAt && <Text dimColor>{untilReset(l.resetsAt, now)}</Text>}
      </Box>
    )

    return (
      <Box flexDirection="column" paddingX={1}>
        <Text bold>Utilizzo settimanale</Text>
        {week ? (
          gauge(week)
        ) : (
          <Text dimColor>Nessun dato ancora (serve un abbonamento e almeno una risposta).</Text>
        )}
        {others.map(gauge)}

        <Box flexDirection="column" marginTop={1}>
          <Text bold>Token della sessione: {fmt(total(m.session))}</Text>
          {row('Input', fmt(m.session.input))}
          {row('Output', fmt(m.session.output))}
          {row('Cache letta', fmt(m.session.cacheRead))}
          {row('Cache scritta', fmt(m.session.cacheWrite))}
          {row('Richieste', `${m.session.requests}`)}
          {m.usd !== undefined && row('Costo stimato', `$${m.usd.toFixed(2)}`)}
        </Box>

        <Box flexDirection="column" marginTop={1}>
          <Text bold>Ultimo turno: {fmt(total(m.lastTurn))}</Text>
          {row('Output', fmt(m.lastTurn.output))}
          {row('Richieste', `${m.lastTurn.requests}`)}
        </Box>

        {m.contextWindow !== undefined && (
          <Box flexDirection="column" marginTop={1}>
            <Text bold>Contesto</Text>
            <Box flexDirection="row">
              <Text color={color(m.contextPercent ?? 0)}>{bar(m.contextPercent ?? 0, width)}</Text>
              <Text> {m.contextPercent ?? 0}%</Text>
            </Box>
            <Text dimColor>
              {fmt(m.contextTokens ?? 0)} / {fmt(m.contextWindow)}
            </Text>
          </Box>
        )}

        {m.model && (
          <Box marginTop={1}>
            <Text dimColor>{m.model}</Text>
          </Box>
        )}
      </Box>
    )
  })
}
