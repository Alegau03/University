# CRITERI
## Authentication
Posticipare il sign-in il più possibile
Spiegarne il perché e i benefici
Minimizzare il data entry

## Data Entry
- noioso ed error prone
- selezionare invece di scrivere
- informazioni dal sistema
- defaults
- validazione dinamica
- minimizzare i campi obbligatori
- hints

## Gestures
- usare gesti standard
- non associare azioni non standard a gesti standard

## Modality
- viste che focalizzano l’attenzione dell’utente
- possono occupare lo schermo in parte o del tutto
- mostrano controlli per il completamento o annullamento
- task modali: pochi, semplici e brevi

## Navigation
- mantenere chiaro il percorso
- nav bar, tab bar, page control
- standard controls / touch gestures

## Branding
- non invasivo
- incorporatovnella grafica
- coerente in tutta l’app
- logo
- non nella nav bar

## Launch
- riprendere lavoro interrotto
- contesto
- tempo necessario all’app / all’utente

LAUNCH SCREEN
- identico
- no testi (localizzazione)



# STRUMENTI
## Haptic Touch
- gesto: pressione prolungata con feedback tattile
- risposta: mostrare un menu, del contenuto aggiuntivo, una preview, un’animazione, etc.

## 3D Touch: quick action
- pressione prolungata su un’icona in home screen
- menu di azioni rapide

## Haptic Touch interactions
(e.g., preview and actions)
![[Screenshot 2024-12-02 alle 16.06.10.png |center |500 |500]]
- per mostrare più contenuto
- live

## Notifiche
- locali o push
- titolo, testo, suono. no nome e logo app
- badge, payload
- banner, alert, lock screen
- apertura —tap sul banner o swipe in lock screen
- annullamento — swipe up o attesa sparizione
- expanded detail view dinamica — 3D o swipe down

## Home Screen Widgets
- per mostrare informazioni o funzionalità essenziali dell’app
- bottoni, testo, immagini, etc.
- sopra le quick action nella schermata Home o nella vista Oggi
- più widget per app
- efficienti nel mostrare il contenuto: cache

## Siri
- tutte le app possono integrarsi con Siri
- utilizzare App Intents per definire i task supportati, validare l’input, eseguire il task e restituire un feedback
- rispondere rapidamente
- offrire un'esperienza fluida e integrata con Siri Shortcuts (richiedere chiarimenti offrendo opzioni per l’utente)



# BARS, VIEWS e CONTROLS
## Status bar
- mostra stato corrente
- automaticamente fornita
- deve rimanere sempre visibile
- progress indeterminato per attività di rete nelle operazioni lunghe

## Navigation bar
- back
- titolo
- segmented control
- un altro bottone
- non serve se non si può navigare dalla vista corrente
- large titles per una gerarchia visiva migliorata

## Toolbar
- contiene bottoni o label per azioni rilevanti nella vista corrente
- deve essere compatta, chiara
- max 5 icone da scegliere in un insieme predefinito. senza didascalia
- max 3 label di testo (dinamiche)

## Tab bar
- per navigare nelle diverse sezioni
- massimo 5 tab, poi “more”
- icona custom + label
- sempre presente
- tab non attivi: mostrare informazioni
- tab sempre visibili e supporto badge dinamici

## Tabelle
- mostrano contenuto, utili per la navigazione
- singola colonna; scrollabili
- elementi raggruppabili
- plain (fig. in alto), grouped (fig. in basso)
- aggiornarle spesso

### Righe delle tabelle
![[Screenshot 2024-12-02 alle 16.12.40.png |center |500 |500]]
![[Screenshot 2024-12-02 alle 16.12.57.png |center |500 |500]]


## Alerts
- asincroni
- solo per informazioni critiche
- interrompono il task in corso
- titolo e testo brevi
- 1 o più bottoni
- l'utente può dover prendere decisioni
- possibilità di annullare

## Action Sheets
- sollecitati dall'utente
- opzioni relative all'azione corrente
- no contenuti scrollabili
- layout chiaro
- bottone per annullare
- task distruttivi evidenziati

## Activity Views
- per eseguire un task utile nel contesto attuale
- fornite di default
- personalizzabili
- usare action button

## Controls
![[Screenshot 2024-12-02 alle 16.15.00.png |center |500 |500]]

### Controls: text fields
- usare la tastiera più appropriata
- supportare input predittivi



# Human Interface Guidelines
## Overview
- layout
	- collections
	- lists and tables
- actions
	- buttons
- navigation
	- navigation bars
	- segmented controls
	- side bars
	- tab bars
- presentation
	- actions sheets
	- alerts

## Collections
“una raccolta gestisce un insieme ordinato di di contenuti e li presenta in un personalizzabile e altamente visivo”
- una riga orizzontale o una griglia
- toccare per selezionare, toccare e tenere premuto per modificare e swipe per scorrere
- ideale per mostrare contenuti basati su immagini per mostrare contenuti basati su immagini; utilizzare una tabella per il testo

## Lists and Tables
- presentare i dati in una o più colonne di righe
- Il formato basato sulle righe è particolarmente adatto per rendere il testo facile da scansionare e leggere 
- selezionare, aggiungere, cancellare e riordino
- con elementi di dimensioni molto diverse
- o se dovete visualizzare un gran numero di immagini numero di immagini, considerate la possibilità di utilizzare una raccolta

## Buttons
- avvia un'azione istantanea
- includere uno spazio sufficiente intorno al pulsante in modo che le persone possano distinguerlo visivamente distinguerlo dai componenti e dai contenuti circostanti
- include sempre un'etichetta di testo o un simbolo (o icona dell'interfaccia) - e a volte una combinazione di entrambi
- mantenere il numero di pulsanti visivamente visivamente in evidenza a uno o due pulsanti per visualizzazione

## Floating Action Buttons
- widget di base in Android
- **non raccomandato su iOS**
- preferire un pulsante d'azione in alto a destra nella barra navale
- Gli utenti iOS non non sapranno come usarlo

## Navigation bars
- un luogo naturale per visualizzare un titolo che può aiutare le persone a orientarsi nell'applicazione
- **utilizzare il pulsante indietro standard**
- considerare l'utilizzo di un controllo in una barra di navigazione per appiattire la gerarchia delle informazioni

## Segmented controls
- numero limitato di opzioni correlate
- opzioni facilmente confrontabili
- alternare viste o modalità all'interno della stessa schermata
- testo, icona, o entrambi
- idealmente 2-5
- modifiche di stato animate per dare un feedback immediato

## Side bars
- possono aiutare le persone a navigare nella vostra fornendo un accesso rapido alle raccolte di raccolte di contenuti di primo livello
- un elenco delle aree e delle raccolte di primo livello dell'app collezioni
- lasciare che le persone nascondano e mostrino la barra laterale; si aspettano di utilizzare il gesto di scorrimento del bordo integrato
- mostrare non più di due livelli di gerarchia in una barra laterale
- **in un'applicazione iOS, considerare l'uso di una barra delle schede invece di una barra laterale**

## Tab bars
- utilizzare una barra delle schede per supportare navigazione, non per fornire azioni
- La barra delle schede consente di navigare tra le diverse aree di un'applicazione
- utilizzare il numero minimo di schede necessario per aiutare le persone a navigare l'applicazione
- mantenere le schede visibili anche quando il loro contenuto non è disponibile
- utilizzare un termine sintetico per ogni scheda titolo
- evitate un termine generico come Home

## Actions sheets
- utilizzare **al posto di un avviso** o di un menu per offrire scelte relative a un'azione azione intenzionale (**prevista**)
- esempio: annullare un messaggio di posta
- posizionare il pulsante di annullamento in in fondo al foglio d'azione
- conferma, annullamento e altre scelte relative all'azione
- evitare di lasciare che un foglio d'azione scorrere

## Alerts
- modale che fornisce informazioni importanti informazioni importanti relative all'applicazione
- informazioni critiche (**inaspettate**) di cui gli utenti hanno bisogno immediatamente
- evitare di fornire solo informazioni
- evitare le azioni comuni e non azioni, anche quando sono distruttive
- può informare le persone di un problema, avvertirli di non distruggere i dati, confermare un acquisto
