export type Tokens = {
  input: number
  output: number
  cacheRead: number
  cacheWrite: number
  requests: number
}

export type Limit = { kind: string; percentUsed: number; resetsAt?: string }

export type Meter = {
  session: Tokens
  lastTurn: Tokens
  usd?: number
  contextPercent?: number
  contextTokens?: number
  contextWindow?: number
  limits: Limit[]
  model?: string
  updatedAt?: number
}

declare module 'claude-code' {
  interface PluginState {
    'token-meter': { meter: Meter }
  }
}
