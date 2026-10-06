# Kanto: testi da scrivere

Generato da `tools/kanto/list_placeholders.py` (rilanciarlo dopo ogni consegna dei testi).

* `[TESTO: …]`: segnaposto con la descrizione di cosa deve dire il testo (trama, allenatori, Palestre, Lega, gating). 0 etichette.
* solo etichetta: segnaposto generico del port (NPC, cartelli, oggetti), "(testo Kanto da scrivere)". 0 etichette.

Regole: una etichetta = un testo; non rinominare le etichette; ~36 caratteri per riga; `\n` nuova riga, `\l` scorre, `\p` nuovo riquadro; chiudere con `$`. Nei blocchi `@ KANTO_V2 BEGIN/END` i testi già scritti (senza "[TESTO:") vengono conservati se si rilancia `kanto_story.py`.
