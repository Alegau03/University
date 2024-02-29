# Progetto Azienda 1  
 
## Punto 1  
Il seguente programma deve implementare la gestione degli utenti in un'azienda, implementano nel seguente modo:
 - Ogni dipendente ha un nome,cognome,data di nascita e stipendio.
 - Ogni dipendente ha iniziato a lavorare in un dipartimento in una specifica data
 - Ogni dipartimento ha un ome e un numero di centralino
 - Ogni dipendente lavora in uno o più progetti
 - Ogni progetto ha un nome e un budget  

Deve essere possibile creare un istanza di un dipendente e in una certa data assegnarlo ad un dipartimento specifico, ad ogni dipendente è associato uno e un unico dipartimento nel quale può lavorare.

Ogni dipartimento ha un Capo Dipartimento che è uno tra i dipendenti

Ogni dipendente può lavorare su uno o più progetti indipendenti dal dipartimento


## Punto 2  
 
```mermaid

---
title: Diagramma Classi Progetto Azienda 1
---
classDiagram

class `IMPIEGATO`{
    nome:stringa
    cognome: stringa
    Data di nascita: stringa
    Stipendio: float
    
}
class `DIPARTIMENTO`{
    Nome:stringa
    Centralono: stringa
}
class `INIZIO CONTRATTO`{
    data: Stringa
}
class `PROGETTO AZIENDALE`{
    Nome:stringa
    Budget: float
}

`INIZIO CONTRATTO` "1..1"<-- "1..1" `IMPIEGATO`  :   Lavora Da
`DIPARTIMENTO` "1..1"<-- "1..1" `INIZIO CONTRATTO`  :   Nel Dipartimento
`IMPIEGATO` "1..1"<-- "0..*" `DIPARTIMENTO`  :   Diretto da
`PROGETTO AZIENDALE` "0..*"<-- "0..*" `IMPIEGATO`  :   Partecipa

```
```mermaid

---
title: Diagramma Istanze Progetto Azienda 1
---
classDiagram

class `impiegato_1:IMPIEGATO`{
    nome:"Alessandro"
    cognome: "Gautieri"
    Data di nascita: "27-05-2003"
    Stipendio: 1000000.0
}
class `dipartimento_1: DIPARTIMENTO`{
    Nome:"Studenti"
    Centralono: "069064729"
}
class `contratto_1: INIZIO CONTRATTO`{
    data: "01-01-2023"
}
class `progetto_1:PROGETTO AZIENDALE`{
    Nome: "Progetto BD2"
    Budget: 1000.0
}

`contratto_1: INIZIO CONTRATTO` "1..1"<-- "1..1" `impiegato_1:IMPIEGATO`  :   Lavora Da
`dipartimento_1: DIPARTIMENTO` "1..1"<-- "1..1" `contratto_1: INIZIO CONTRATTO`  :   Nel Dipartimento
`impiegato_1:IMPIEGATO` "1..1"<-- "0..*" `dipartimento_1: DIPARTIMENTO`  :   Diretto da
`progetto_1:PROGETTO AZIENDALE` "0..*"<-- "0..*" `impiegato_1:IMPIEGATO`  :   Partecipa
```
