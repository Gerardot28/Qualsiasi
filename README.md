# Qualsiasi

## token-meter

Una mod per Claude Code che apre una barra laterale con, in tempo reale:

- la **percentuale di utilizzo settimanale** (limite di 7 giorni) e quanto manca al reset, più il limite di 5 ore quando c'è;
- i **token spesi** nella sessione (input, output, cache letta e scritta, richieste) e il costo stimato;
- i token dell'**ultimo turno** e quanto è pieno il **contesto**.

La barra si apre all'avvio e si riapre a ogni messaggio che invii. Sotto il prompt resta anche una riga di stato, per esempio `⚡ 48.2k tok · sett. 42.5% · $1.23`. Se chiudi la barra, `/token-meter` la riapre.

### Installazione

Nel terminale, al prompt di Claude Code:

```
/plugin install token-meter --marketplace Gerardot28/Qualsiasi
```

Rispondi `y` per aggiungere il marketplace, poi scegli l'ambito (premi Invio per quello utente). La mod si attiva subito e in tutte le sessioni successive.

### Note

- La percentuale settimanale c'è solo con un abbonamento Claude (Pro/Max) e compare dopo la prima risposta.
- La barra si piazza da sola quando la finestra è abbastanza larga; altrimenti aprila con `/token-meter`. La riga di stato si vede sempre.
