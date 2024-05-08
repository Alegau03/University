/*
1 ) Quali sono i cognomi distinti di tutti gli strutturati?

select distinct persona.cognome
from persona
*/

/*
2 ) Quali sono i ricercatori ( con nome e cognome )?

select persona.nome, persona.cognome
from persona
where persona.posizione = 'Ricercatore'
*/

/*
3) Quali sono i Professori Associati il cui cognome comincia con la lettera ‘V’?

select persona.nome, persona.cognome
from persona
where persona.posizione = 'Professore Associato' and cognome like 'V%'
*/

/*
4) Quali sono i Professori (sia Associati che Ordinari) il cui cognome comincia con la
lettera ‘V’?

select persona.nome, persona.cognome
from persona
where persona.cognome like 'V%' and (persona.posizione = 'Professore Associato' or persona.posizione = 'Professore Ordinario') 
*/

/*
5) Quali sono i Progetti già terminati alla data odierna?

select *
from progetto
where progetto.fine < '08/05/2024'
*/

/*
6) Quali sono i nomi di tutti i Progetti ordinati in ordine crescente di data di inizio?

select progetto.nome
from progetto
order by inizio asc
*/

/*
7) Quali sono i nomi dei WP ordinati in ordine crescente (per nome)?

select *
from wp
order by nome asc
*/

/*
8)  Quali sono (distinte) le cause di assenza di tutti gli strutturati?

select distinct assenza.tipo
from assenza
*/

/*
9) Quali sono (distinte) le tipologie di attività di progetto di tutti gli strutturati?

select distinct attivitaprogetto.tipo
from attivitaprogetto
*/

/*
10)  Quali sono i giorni distinti nei quali del personale ha effettuato attività non progettuali di tipo ‘Didattica’? Dare il risultato in ordine crescente.

select distinct attivitanonprogettuale.giorno
from attivitanonprogettuale
where attivitanonprogettuale.tipo = 'Didattica'
order by giorno asc
*/

