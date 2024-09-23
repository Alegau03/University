/* 1) Quanti sono gli strutturati di ogni fascia?*/

select p.posizione as Posizione, count(p.posizione) as Numero
from persona p
group by p.posizione
order by p.posizione desc


/* 2) Quanti sono gli strutturati con stipendio ≥ 40000? */

SELECT COUNT(*) AS numero
from persona
where persona.stipendio>=40000


/* 3) Quanti sono i progetti già finiti che superano il budget di 50000?*/


select count(*) as numero
from progetto
where progetto.fine<'2024-05-18' and progetto.budget>500


/* 4) Qual è la media, il massimo e il minimo delle ore delle attività relative al progetto*/
‘Pegasus’?

select avg(a.oredurata) as media, max(a.oredurata) as massimo, min(a.oredurata) as minimo
from attivitaprogetto a
where a.progetto='pegasus'


/* 5) Quali sono le medie, i massimi e i minimi delle ore giornaliere dedicate al progetto*/
‘Pegasus’ da ogni singolo docente? 
select p.id as id, p.nome as nome, p.cognome as cognome, avg(a.oredurata) as media, max(a.oredurata) as massimo, min(a.oredurata) as minimo
from attivitaprogetto a, persona p, progetto prog
where a.progetto=prog.id and prog.nome='Pegasus' and (p.posizione='Professore Associato' or p.posizione='Professore Ordinario')
group by p.id


/* 6)  Qual è il numero totale di ore dedicate alla didattica da ogni docente?*/

SELECT p.id, p.nome, p.cognome, SUM(a.oreDurata) AS totale_ore_didattica
FROM Persona p, attivitanonprogettuale a
WHERE a.tipo = 'Didattica' and p.id=a.persona
GROUP BY p.id, p.nome, p.cognome;


/* 7) Qual è la media, il massimo e il minimo degli stipendi dei ricercatori? */

select avg(p.stipendio) as media,max(p.stipendio) as massimo, min(p.stipendio) as minimo
from persona p
where p.posizione='Ricercatore'


/* 8) Quali sono le medie, i massimi e i minimi degli stipendi dei ricercatori, dei professori*/
associati e dei professori ordinari? 
select p.posizione as posizione, avg(p.stipendio) as media,max(p.stipendio) as massimo, min(p.stipendio) as minimo
from persona p
group by p.posizione


/* 9) Quante ore ‘Ginevra Riva’ ha dedicato ad ogni progetto nel quale ha lavorato?*/

select prog.nome as progetto, sum(a.oreDurata) as numero_ore
from progetto prog, persona p, attivitaprogetto a
where (p.nome='Ginevra' and p.cognome='Riva')and a.progetto=prog.id and a.persona=p.id
group by prog.nome, a.progetto


/* 10) Qual è il nome dei progetti su cui lavorano più di due strutturati?*/

select p.id, p.nome
from progetto p, persona pers, attivitaprogetto a
where p.id=a.progetto and a.persona=pers.id
group by p.id
having count(distinct pers.id)>2


/* 11) Quali sono i professori associati che hanno lavorato su più di un progetto?*/

SELECT pers.id, pers.nome, pers.cognome
FROM Persona pers, AttivitaProgetto a
WHERE pers.posizione = 'Professore Associato' and pers.id=a.persona
GROUP BY pers.id, pers.nome, pers.cognome
HAVING COUNT(DISTINCT a.progetto) > 1;
