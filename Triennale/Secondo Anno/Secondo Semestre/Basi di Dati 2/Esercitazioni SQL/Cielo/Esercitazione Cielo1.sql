/* 1) Quali sono i voli (codice e nome della compagnia) la cui durata supera le 3 ore?

select distinct volo.codice, compagnia.nome
from volo, compagnia
where volo.durataMinuti>180 and volo.comp=compagnia.nome*/

/* 2) Quali sono le compagnie che hanno voli che superano le 3 ore?
select distinct compagnia.nome
from volo, compagnia
where volo.durataMinuti>180 and volo.comp=compagnia.nome*/

/* 3) Quali sono i voli (codice e nome della compagnia) che partono dall’aeroporto con
codice ‘CIA’?

select distinct compagnia.nome,volo.codice, a.partenza
from volo, compagnia, aeroporto, arrpart a
where volo.comp=compagnia.nome and a.comp=compagnia.nome and a.partenza='CIA' and aeroporto.codice=a.arrivo
*/

/* 4) Quali sono le compagnie che hanno voli che arrivano all’aeroporto con codice
‘FCO’? 

select distinct compagnia.nome
from arrpart ap, compagnia, aeroporto 
where compagnia.nome=ap.comp and ap.arrivo=aeroporto.codice and aeroporto.codice='FCO'
*/

/* 5) Quali sono i voli (codice e nome della compagnia) che partono dall’aeroporto ‘FCO’
e arrivano all’aeroporto ‘JFK’?

SELECT V.codice, V.comp
FROM Volo V
JOIN ArrPart AP ON V.codice = AP.codice AND V.comp = AP.comp
WHERE AP.partenza = 'FCO' AND AP.arrivo = 'JFK';
*/
/* 6) Quali sono le compagnie che hanno voli che partono dall’aeroporto ‘FCO’ e atterrano
all’aeroporto ‘JFK’?
SELECT V.comp
FROM Volo V
JOIN ArrPart AP ON V.codice = AP.codice AND V.comp = AP.comp
WHERE AP.partenza = 'FCO' AND AP.arrivo = 'JFK';
*/
/* 7) Quali sono i nomi delle compagnie che hanno voli diretti dalla città di ‘Roma’ alla
città di ‘New York’?
SELECT DISTINCT C.nome
FROM Volo V
JOIN ArrPart AP ON V.codice = AP.codice AND V.comp = AP.comp
JOIN Aeroporto A1 ON AP.partenza = A1.codice
JOIN LuogoAeroporto LA1 ON A1.codice = LA1.aeroporto
JOIN Aeroporto A2 ON AP.arrivo = A2.codice
JOIN LuogoAeroporto LA2 ON A2.codice = LA2.aeroporto
JOIN Compagnia C ON V.comp = C.nome
WHERE LA1.citta = 'Roma' AND LA2.citta = 'New York';
*/

/* 8) Quali sono gli aeroporti (con codice IATA, nome e luogo) nei quali partono voli
della compagnia di nome ‘MagicFly’?

select la.citta as città, aeroporto.codice as codice, aeroporto.nome as Nome
from luogoaeroporto la, aeroporto, compagnia, arrpart ap
where ap.comp=compagnia.nome and compagnia.nome='MagicFly' and ap.partenza=aeroporto.codice and aeroporto.codice=la.aeroporto
*/

/* 9) Quali sono i voli che partono da un qualunque aeroporto della città di ‘Roma’ e
atterrano ad un qualunque aeroporto della città di ‘New York’? Restituire: codice
del volo, nome della compagnia, e aeroporti di partenza e arrivo.

SELECT DISTINCT  V.codice as CodiceVolo,C.nome as NomeCompagnia, A1.codice as Partenza, A2.codice as Arrivo
FROM Volo V
JOIN ArrPart AP ON V.codice = AP.codice AND V.comp = AP.comp
JOIN Aeroporto A1 ON AP.partenza = A1.codice
JOIN LuogoAeroporto LA1 ON A1.codice = LA1.aeroporto
JOIN Aeroporto A2 ON AP.arrivo = A2.codice
JOIN LuogoAeroporto LA2 ON A2.codice = LA2.aeroporto
JOIN Compagnia C ON V.comp = C.nome
WHERE LA1.citta = 'Roma' AND LA2.citta = 'New York';
*/
/* 10) Quali sono i possibili piani di volo con esattamente un cambio (utilizzando solo
voli della stessa compagnia) da un qualunque aeroporto della città di ‘Roma’ ad un
qualunque aeroporto della città di ‘New York’? Restituire: nome della compagnia,
codici dei voli, e aeroporti di partenza, scalo e arrivo.*/
???
/* 11) Quali sono le compagnie che hanno voli che partono dall’aeroporto ‘FCO’, atterrano
all’aeroporto ‘JFK’, e di cui si conosce l’anno di fondazione?

SELECT compagnia.nome AS NomeCompagnia, compagnia.annoFondaz AS AnnoFondazione
FROM Volo V
JOIN ArrPart AP ON V.codice = AP.codice AND V.comp = AP.comp
JOIN compagnia ON V.comp = compagnia.nome
WHERE AP.partenza = 'FCO' AND AP.arrivo = 'JFK' AND compagnia.annoFondaz IS NOT NULL;
*/
