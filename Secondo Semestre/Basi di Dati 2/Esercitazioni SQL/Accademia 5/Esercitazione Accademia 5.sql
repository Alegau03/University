/*#######################################################################################
1)  Quali sono il nome, la data di inizio e la data di fine dei WP del progetto di nome
‘Pegasus’?

select wp.nome, wp.inizio, wp.fine
from wp, progetto
where progetto.nome = 'Pegasus' 
and wp.progetto= progetto.id
#######################################################################################*/

/* #######################################################################################
2) Quali sono il nome, il cognome e la posizione degli strutturati che hanno almeno
una attività nel progetto ‘Pegasus’, ordinati per cognome decrescente? 

select distinct persona.nome, persona.cognome, persona.posizione
from persona, progetto, attivitaprogetto
where attivitaprogetto.persona = persona.id 
and attivitaprogetto.progetto=progetto.id and progetto.nome = 'Pegasus'
order by persona.cognome desc
#######################################################################################*/

/*#######################################################################################
3) Quali sono il nome, il cognome e la posizione degli strutturati che hanno più di
una attività nel progetto ‘Pegasus’? 

select distinct persona.nome, persona.cognome, persona.posizione
from persona, progetto, attivitaprogetto as at1, attivitaprogetto as at2
where at1.persona = persona.id 
and at1.progetto=progetto.id and progetto.nome = 'Pegasus'
and at1.persona = persona.id and at2.persona = persona.id
and at1 <>at2
order by persona.cognome desc
#######################################################################################*/

/*####################################################################################### 
4) Quali sono il nome, il cognome e la posizione dei Professori Ordinari che hanno
fatto almeno una assenza per malattia?

select distinct  persona.nome, persona.cognome, persona.posizione
from persona, assenza
where persona.posizione = 'Professore Ordinario'
and assenza.persona=persona.id and assenza.tipo= 'Malattia' 
#######################################################################################*/

/*####################################################################################### 
5) Quali sono il nome, il cognome e la posizione dei Professori Ordinari che hanno
fatto più di una assenza per malattia? 

select distinct persona.nome, persona.cognome, persona.posizione
from persona, assenza as a1, assenza as a2
where persona.posizione = 'Professore Ordinario'
and a1.persona=persona.id and a1.tipo= 'Malattia' and a2.persona=persona.id 
#######################################################################################*/

/*#######################################################################################
6) Quali sono il nome, il cognome e la posizione dei Ricercatori che hanno almeno
un impegno per didattica?

select distinct persona.nome, persona.cognome, persona.posizione
from persona, attivitanonprogettuale as attivita
where persona.id = attivita.persona
and attivita.tipo='Didattica'
and persona.posizione='Ricercatore'
#######################################################################################*/

/*####################################################################################### 
7) Quali sono il nome, il cognome e la posizione dei Ricercatori che hanno più di un
impegno per didattica? 

select distinct persona.nome, persona.cognome, persona.posizione
from persona, attivitanonprogettuale as at1, attivitanonprogettuale as at2
where at1.persona = persona.id 
and at1.persona = persona.id and at2.persona = persona.id
and at1 <>at2
and persona.posizione='Ricercatore'
and at1.tipo='Didattica'
#######################################################################################*/

/*#######################################################################################
8) Quali sono il nome e il cognome degli strutturati che nello stesso giorno hanno sia
attività progettuali che attività non progettuali?

select persona.nome, persona.cognome
from persona,attivitaprogetto as ap, attivitanonprogettuale as anp
where persona.id= ap.persona and persona.id=anp.persona
and ap.giorno=anp.giorno
#######################################################################################*/


/*#######################################################################################
9) Quali sono il nome e il cognome degli strutturati che nello stesso giorno hanno sia
attività progettuali che attività non progettuali? Si richiede anche di proiettare il
giorno, il nome del progetto, il tipo di attività non progettuali e la durata in ore di
entrambe le attività. 

select persona.nome, persona.cognome, ap.giorno, progetto.nome, ap.oredurata,anp.tipo, anp.oredurata
from persona,attivitaprogetto as ap, attivitanonprogettuale as anp, progetto
where persona.id= ap.persona and persona.id=anp.persona
and ap.giorno=anp.giorno
and ap.progetto = progetto.id
#######################################################################################*/

/*#######################################################################################
10) Quali sono il nome e il cognome degli strutturati che nello stesso giorno sono
assenti e hanno attività progettuali?

select persona.nome, persona.cognome
from persona,attivitaprogetto  as ap, assenza
where persona.id= ap.persona
and assenza.persona=persona.id
and assenza.giorno=ap.giorno
#######################################################################################*/

/*#######################################################################################
11) Quali sono il nome e il cognome degli strutturati che nello stesso giorno sono
assenti e hanno attività progettuali? Si richiede anche di proiettare il giorno, il
nome del progetto, la causa di assenza e la durata in ore della attività progettuale.

select persona.nome, persona.cognome, assenza.giorno, progetto.nome, assenza.tipo, ap.oredurata
from persona,attivitaprogetto  as ap, assenza, progetto
where persona.id= ap.persona
and assenza.persona=persona.id
and assenza.giorno=ap.giorno
and ap.progetto=progetto.id
#######################################################################################*/

/*#######################################################################################
12) Quali sono i WP che hanno lo stesso nome, ma appartengono a progetti diversi?

select distinct w1.nome
from wp as w1, wp as w2
where w1.nome=w2.nome 
and w1.progetto!=w2.progetto
#######################################################################################*/

