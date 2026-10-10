import { describe, expect, mock, test } from 'claude-code/testing'

import { bar, fmt, untilReset } from '../hooks/register'

const PANE = {
  component: 'Pane',
  requestId: 'token-meter',
  props: {
    title: 'Token & limiti',
    isFocused: false,
    bodyColumns: 40,
    placement: 'dock',
    scroll: { offset: 0, bodyRows: 30 },
    view: {},
  },
} as const

describe('token-meter', () => {
  test('formats numbers and bars', async () => {
    expect(fmt(950)).toBe('950')
    expect(fmt(12_345)).toBe('12.3k')
    expect(fmt(2_500_000)).toBe('2.50M')
    expect(bar(50, 10)).toBe('█████░░░░░')
    expect(untilReset('1970-01-02T01:00:00Z', 0)).toBe('reset tra 1g 1h')
  })

  test('shows the weekly percentage pushed by session.measure', async ($, on) => {
    mock.clock(on)
    on('session.measure', (_$, e) => ({ changed: e.changed }))
    await $.session.measure({
      context: { window: 200_000, tokens: 50_000, percent: 25 },
      rateLimits: [
        { kind: 'five_hour', percentUsed: 12 },
        { kind: 'seven_day', percentUsed: 42.5 },
      ],
      cost: { usd: 1.23 },
      changed: ['context', 'rateLimits', 'cost'],
    })

    for (const surface of ['terminal', 'desktop', 'vscode', 'mobile'] as const) {
      const ui = await $.ui.mount({ plugin: 'token-meter', surface, ...PANE })
      expect(await ui.find({ type: 'Text', text: /42\.5%/ })).toBeDefined()
      expect(await ui.find({ type: 'Text', text: /Settimanale/ })).toBeDefined()
      expect(await ui.find({ type: 'Text', text: /\$1\.23/ })).toBeDefined()
      await ui.unmount()
    }
  })
})
