/* 1) Quante sono le compagnie che operano (sia in arrivo che in partenza) nei diversi
aeroporti? 

SELECT aeroporto, COUNT(DISTINCT comp) AS numero_compagnie
FROM (
		SELECT partenza AS aeroporto, comp 
			FROM ArrPart
    UNION
    	SELECT arrivo AS aeroporto, comp 
			FROM ArrPart
) AS OperazioniAeroporto
GROUP BY aeroporto;
*/
/* 2)  Quanti sono i voli che partono dall’aeroporto ‘HTR’ e hanno una durata di almeno
100 minuti?

select count(volo.codice) as numero_voli
from volo join arrpart on arrpart.codice=volo.codice
join aeroporto on arrpart.partenza=aeroporto.codice
where aeroporto.codice='HTR' and volo.durataminuti>100
*/

/* 3) Quanti sono gli aeroporti sui quali opera la compagnia ‘Apitalia’, per ogni nazione
nella quale opera 


SELECT L.nazione, COUNT(DISTINCT A.codice) AS numero_aeroporti
FROM Volo V
JOIN ArrPart AP ON V.codice = AP.codice AND V.comp = AP.comp
JOIN Aeroporto A ON AP.arrivo = A.codice OR AP.partenza = A.codice
JOIN LuogoAeroporto L ON A.codice = L.aeroporto
WHERE V.comp = 'Apitalia'
GROUP BY L.nazione;
*/

/* 4) Qual è la media, il massimo e il minimo della durata dei voli effettuati dalla
compagnia ‘MagicFly’? 

select avg(volo.durataminuti) as media, max(volo.durataminuti) as massimo, min(volo.durataminuti) as minimo
from volo 
join arrpart on volo.codice=arrpart.codice and volo.comp=arrpart.comp
join compagnia on volo.comp=compagnia.nome
where compagnia.nome='MagicFly'
*/

/* 5) Qual è l’anno di fondazione della compagnia più vecchia che opera in ognuno degli
aeroporti? 
SELECT A.codice, A.nome, MIN(C.annoFondaz) AS anno
FROM ArrPart AP
JOIN Volo V ON AP.codice = V.codice AND AP.comp = V.comp
JOIN Compagnia C ON V.comp = C.nome
JOIN Aeroporto A ON AP.arrivo = A.codice OR AP.partenza = A.codice
WHERE C.annoFondaz IS NOT NULL
GROUP BY A.codice, A.nome
ORDER BY A.nome;
*/

/* 6) Quante sono le nazioni (diverse) raggiungibili da ogni nazione tramite uno o più
voli?

SELECT la_partenza.nazione AS nazione_partenza, COUNT(DISTINCT la_arrivo.nazione) AS nazioni_raggiungibili
FROM ArrPart AP
JOIN LuogoAeroporto la_partenza ON AP.partenza = la_partenza.aeroporto
JOIN LuogoAeroporto la_arrivo ON AP.arrivo = la_arrivo.aeroporto
GROUP BY la_partenza.nazione;
*/ 

/* 7) Qual è la durata media dei voli che partono da ognuno degli aeroporti?

SELECT A.codice, A.nome, avg(v.durataminuti) AS media
FROM ArrPart AP
JOIN Volo V ON AP.codice = V.codice AND AP.comp = V.comp
JOIN Aeroporto A ON AP.partenza = A.codice
GROUP BY A.codice, A.nome
ORDER BY A.nome;
*/

/* 8) Qual è la durata complessiva dei voli operati da ognuna delle compagnie fondate
a partire dal 1950? 
SELECT V.comp, sum(V.durataminuti) AS conto_durata
FROM ArrPart AP
JOIN Volo V ON AP.codice = V.codice AND AP.comp = V.comp
JOIN Compagnia C ON V.comp = C.nome
WHERE C.annoFondaz>=1950
GROUP BY V.comp
*/

/* 9) Quali sono gli aeroporti nei quali operano esattamente due compagnie?
SELECT A.codice, A.nome
FROM ArrPart AP
JOIN Aeroporto A ON AP.partenza = A.codice OR AP.arrivo = A.codice
GROUP BY A.codice, A.nome
HAVING COUNT(DISTINCT AP.comp) = 2;
*/
	
/* 10) Quali sono le città con almeno due aeroporti?

select la.citta
from luogoaeroporto la 
join aeroporto A on la.aeroporto=A.codice
group by la.citta
having count(la.citta)>=2;
*/

/* 11) Qual è il nome delle compagnie i cui voli hanno una durata media maggiore di 6
ore?

select C.nome
from compagnia C
join Volo V on c.nome=V.comp
group by c.nome
having avg(v.durataminuti)>360
*/

/* 12) Qual è il nome delle compagnie i cui voli hanno tutti una durata maggiore di 100
minuti?*/

