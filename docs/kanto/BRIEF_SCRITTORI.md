# Brief per gli scrittori dei testi di Kanto (Atto 2)

Leggi PRIMA: `docs/storia/bibbia_kanto.md` (trama Atto 2, cast, scene), `docs/storia/kanto_scene.json`,
la guida di stile in `docs/storia/bibbia.md` (sezione stile + glossario) e `docs/storia/nomi.json`.

Compito: in ogni file assegnato (in /home/user/pex) sostituisci OGNI testo segnaposto
- `[TESTO: descrizione]` → scrivi esattamente ciò che la descrizione chiede (trama, lotte, Palestre, Lega, blocchi);
- `(testo Kanto da scrivere)` → testo originale per quell'NPC/cartello/oggetto. Il file originale FRLG
  `scripts_frlg_orig.inc` nella stessa cartella mostra cosa diceva quel personaggio (in inglese): usalo come
  ispirazione per ruolo e informazioni utili, ma scrivi testo NUOVO in italiano coerente con l'Atto 2
  (multiverso, Varchi, Team Rocket + Serratura Silph, Morgana, Arianna). Nomi ufficiali italiani dei luoghi.

Regole tecniche (come per Hoenn):
1. Cambia SOLO il contenuto delle righe `.string "..."`; puoi aggiungere righe `.string` dentro lo stesso blocco.
   Non toccare etichette, comandi, flag, `@ KANTO_V2` marker.
2. Il blocco termina con `$` come prima. Riquadro di 2 righe: prima interruzione `\n`, poi `\l`, `\p` nuovo riquadro.
   Larghezza max 208 px (216 ultima riga); testi di sconfitta in lotta 200/208 px.
   Misura: `python3 -I /home/user/Qualsiasi/tools/textcheck/measure.py "riga"`.
3. Caratteri: apostrofo ' ; virgolette “ ” ; niente « » – — ° [ ] ; accenti precomposti.
4. Verifica ogni file: `python3 -I /home/user/Qualsiasi/tools/textcheck/textcheck.py --single /home/user/pex/<file> /home/user/work/kanto-ref/<file>`
   → 0 errori. Nessun segnaposto deve restare (`grep -c "TESTO\|testo Kanto da scrivere"` = 0).
5. Domande del quiz di Isola Cannella: le risposte corrette indicate nel segnaposto (Q1 sì, 2 no, 3 no, 4 no, 5 sì, 6 no) devono restare corrette.
6. Non eseguire make. Non modificare altri file.
