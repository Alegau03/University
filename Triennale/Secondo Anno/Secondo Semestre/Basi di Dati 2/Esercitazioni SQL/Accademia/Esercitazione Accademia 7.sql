/*1. Qual è media e deviazione standard degli stipendi per ogni categoria di strutturati?

SELECT posizione, 
       AVG(stipendio) AS media_stipendio, 
       STDDEV(stipendio) AS deviazione_standard_stipendio
FROM Persona
GROUP BY posizione;
*/

/*2. Quali sono i ricercatori (tutti gli attributi) con uno stipendio superiore alla media
della loro categoria?

SELECT p.id, p.nome, p.cognome, p.posizione, p.stipendio
FROM Persona p
JOIN (
    SELECT posizione, AVG(stipendio) AS media_stipendio
    FROM Persona
    GROUP BY posizione
) media ON p.posizione = media.posizione
WHERE p.stipendio > media.media_stipendio
  AND p.posizione = 'Ricercatore';
*/

/*3. Per ogni categoria di strutturati quante sono le persone con uno stipendio che
differisce di al massimo una deviazione standard dalla media della loro categoria?


SELECT p.posizione, count(*) as numero_persone
FROM Persona p
JOIN (
    SELECT posizione, AVG(stipendio) AS media_stipendio, STDDEV(stipendio) AS stddev_stipendio
    FROM Persona
    GROUP BY posizione
) stats ON p.posizione = stats.posizione
WHERE p.stipendio between(stats.media_stipendio - stats.stddev_stipendio) and
							(stats.media_stipendio + stats.stddev_stipendio)

group by p.posizione
*/
/*4. Chi sono gli strutturati che hanno lavorato almeno 20 ore complessive in attività
progettuali? Restituire tutti i loro dati e il numero di ore lavorate.
SELECT p.id, p.nome, p.cognome, p.posizione, p.stipendio, SUM(ap.oreDurata) AS numero_ore
FROM Persona p
JOIN AttivitaProgetto ap ON p.id = ap.persona
GROUP BY p.id, p.nome, p.cognome, p.posizione, p.stipendio
HAVING SUM(ap.oreDurata) >= 20;
*/

/*5. Quali sono i progetti la cui durata è superiore alla media delle durate di tutti i
progetti? Restituire nome dei progetti e loro durata in giorni.

SELECT nome, (fine - inizio) AS durata_giorni
FROM Progetto
WHERE (fine - inizio) > (
    SELECT AVG(fine - inizio)
    FROM Progetto)
*/


/*6. Quali sono i progetti terminati in data odierna che hanno avuto attività di tipo
“Dimostrazione”? Restituire nome di ogni progetto e il numero complessivo delle
ore dedicate a tali attività nel progetto.

select p.nome, sum(ap.oredurata)
from progetto p, (select *
	from attivitaprogetto
	where tipo='Dimostrazione') ap 
where p.id=ap.progetto
group by p.nome
*/

/*7. Quali sono i professori ordinari che hanno fatto più assenze per malattia del numero di assenze medio per malattia dei professori associati? Restituire id, nome e
cognome del professore e il numero di giorni di assenza per malattia.

-- Calcola il numero medio di assenze per malattia per i professori associati
WITH MediaAssenzeAssociati AS (
    SELECT AVG(num_assenze) AS media_assenze
    FROM (
        SELECT persona, COUNT(*) AS num_assenze
        FROM Assenza
        WHERE tipo = 'Malattia'
        AND persona IN (SELECT id FROM Persona WHERE posizione = 'Professore Associato')
        GROUP BY persona
    ) AS subquery
)

-- Seleziona i professori ordinari che hanno fatto più assenze per malattia della media calcolata sopra
SELECT p.id, p.nome, p.cognome, COUNT(*) AS num_assenze_malattia
FROM Persona p
JOIN Assenza a ON p.id = a.persona
WHERE p.posizione = 'Professore Ordinario'
AND a.tipo = 'Malattia'
GROUP BY p.id, p.nome, p.cognome
HAVING COUNT(*) > (SELECT media_assenze FROM MediaAssenzeAssociati);
*/