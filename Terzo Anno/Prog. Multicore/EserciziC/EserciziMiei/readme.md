# Simulazione Problema degli n-Corpi (Versione Seriale)

## Descrizione del Programma
Il programma simula le interazioni gravitazionali tra `n` corpi nello spazio. Ogni corpo è caratterizzato da posizione, velocità e massa. La simulazione è eseguita serialmente ed è composta da:
1. **Inizializzazione**: Generazione di corpi con proprietà casuali.
2. **Simulazione**: Calcolo delle forze gravitazionali, aggiornamento di velocità e posizione per 100 passi temporali.
3. **Benchmark**: Il programma esegue 100 simulazioni complete per calcolare il tempo medio di esecuzione.

## Iterazioni
- **Iterazioni per Benchmark**: 100 esecuzioni per calcolare la media dei tempi.
- **Passi temporali per Simulazione**: Ogni esecuzione simula 100 passi temporali.
- **Totale Iterazioni**: \(100 \times 100 = 10,000\).

## Complessità
Il calcolo delle forze tra i corpi ha complessità \( O(n^2) \), perciò i tempi di esecuzione aumentano rapidamente con il numero di corpi.

## Tabella Benchmark

| Numero di Corpi | Tempo Medio di Esecuzione (secondi) |
|-----------------|-------------------------------------|
| 4               | 0.000052                           |
| 8               | 0.000158                           |
| 16              | 0.000411                           |
| 32              | 0.001082                           |
| 64              | 0.003304                           |
| 128             | 0.012200                           |
| 256             | 0.048403                           |
| 512             | 0.194348                           |
| 1024            | 0.799099                           |


# Benchmark: Confronto Serial vs OpenMP

## Tabella Benchmark OpenMP

| Numero di Corpi | Numero di Thread | Tempo Medio di Esecuzione (secondi) |
|-----------------|------------------|-------------------------------------|
| 4               | 4                | 0.002699                           |
| 8               | 4                | 0.002731                           |
| 16              | 4                | 0.002837                           |
| 32              | 4                | 0.003201                           |
| 64              | 4                | 0.004354                           |
| 128             | 4                | 0.007432                           |
| 256             | 4                | 0.018059                           |
| 512             | 4                | 0.061189                           |
| 1024            | 4                | 0.233798                           |
| 4               | 8                | 0.004218                           |
| 8               | 8                | 0.004258                           |
| 16              | 8                | 0.004321                           |
| 32              | 8                | 0.004530                           |
| 64              | 8                | 0.005330                           |
| 128             | 8                | 0.008235                           |
| 256             | 8                | 0.019120                           |
| 512             | 8                | 0.060958                           |
| 1024            | 8                | 0.207475                           |

---

## Tabella Confronto Serial vs OpenMP (4 Thread)

| Numero di Corpi | Tempo Serial (secondi) | Tempo OpenMP (4 Thread) (secondi) | Velocità OpenMP (% miglioramento) |
|-----------------|-------------------------|-----------------------------------|-----------------------------------|
| 4               | 0.000052               | 0.002699                         | -98.07%                          |
| 8               | 0.000158               | 0.002731                         | -94.21%                          |
| 16              | 0.000411               | 0.002837                         | -85.51%                          |
| 32              | 0.001082               | 0.003201                         | -66.18%                          |
| 64              | 0.003304               | 0.004354                         | -25.38%                          |
| 128             | 0.012200               | 0.007432                         | 39.07%                           |
| 256             | 0.048403               | 0.018059                         | 62.68%                           |
| 512             | 0.194348               | 0.061189                         | 68.51%                           |
| 1024            | 0.799099               | 0.233798                         | 70.73%                           |

---

## Tabella Confronto Serial vs OpenMP (8 Thread)

| Numero di Corpi | Tempo Serial (secondi) | Tempo OpenMP (8 Thread) (secondi) | Velocità OpenMP (% miglioramento) |
|-----------------|-------------------------|-----------------------------------|-----------------------------------|
| 4               | 0.000052               | 0.004218                         | -98.77%                          |
| 8               | 0.000158               | 0.004258                         | -96.29%                          |
| 16              | 0.000411               | 0.004321                         | -90.49%                          |
| 32              | 0.001082               | 0.004530                         | -76.10%                          |
| 64              | 0.003304               | 0.005330                         | -37.73%                          |
| 128             | 0.012200               | 0.008235                         | 32.50%                           |
| 256             | 0.048403               | 0.019120                         | 60.51%                           |
| 512             | 0.194348               | 0.060958                         | 68.63%                           |
| 1024            | 0.799099               | 0.207475                         | 74.04%                           |

---

## Osservazioni

- Per un numero ridotto di corpi (4, 8, 16), OpenMP risulta meno efficiente della versione seriale a causa del sovraccarico della gestione dei thread.
- Per un numero elevato di corpi (128, 256, 512, 1024), OpenMP offre un miglioramento significativo delle prestazioni, fino al **74% di riduzione del tempo di esecuzione** rispetto alla versione seriale.

# Benchmark Confronto: Serial vs OpenMP vs MPI+OpenMP

| Numero di Corpi | Versione Serial (s) | OpenMP (4 Thread) (s) | MPI+OpenMP (4 Proc, 4 Th) (s) | Miglioramento OpenMP (%) | Miglioramento MPI+OpenMP vs Serial (%) | Miglioramento MPI+OpenMP vs OpenMP (%) |
|-----------------|----------------------|-----------------------|------------------------------|--------------------------|---------------------------------------|---------------------------------------|
| 4               | 0.000052            | 0.002699             | 0.009312                    | -98.07%                 | -99.44%                              | -71.04%                              |
| 8               | 0.000158            | 0.002731             | 0.008851                    | -94.21%                 | -98.21%                              | -69.13%                              |
| 16              | 0.000411            | 0.002837             | 0.008151                    | -85.51%                 | -94.95%                              | -65.20%                              |
| 32              | 0.001082            | 0.003201             | 0.009520                    | -66.18%                 | -91.40%                              | -66.35%                              |
| 64              | 0.003304            | 0.004354             | 0.009460                    | -25.38%                 | -71.37%                              | -55.96%                              |
| 128             | 0.012200            | 0.007432             | 0.011613                    | 39.07%                  | 4.81%                                | -56.14%                              |
| 256             | 0.048403            | 0.018059             | 0.020453                    | 62.68%                  | 57.75%                               | -11.71%                              |
| 512             | 0.194348            | 0.061189             | 0.054218                    | 68.51%                  | 72.10%                               | 11.39%                               |
| 1024            | 0.799099            | 0.233798             | 0.172691                    | 70.73%                  | 78.39%                               | 26.11%                               |

---

## Spiegazione delle Colonne
- **Numero di Corpi**: Il numero di corpi simulati.
- **Versione Serial (s)**: Tempo medio di esecuzione della versione seriale.
- **OpenMP (4 Thread) (s)**: Tempo medio di esecuzione della versione OpenMP con 4 thread.
- **MPI+OpenMP (4 Proc, 4 Th) (s)**: Tempo medio di esecuzione della versione MPI + OpenMP con 4 processi e 4 thread per processo.
- **Miglioramento OpenMP (%)**: Riduzione del tempo di esecuzione rispetto alla versione seriale.
- **Miglioramento MPI+OpenMP vs Serial (%)**: Riduzione del tempo di esecuzione della versione MPI + OpenMP rispetto alla versione seriale.
- **Miglioramento MPI+OpenMP vs OpenMP (%)**: Riduzione del tempo di esecuzione della versione MPI + OpenMP rispetto alla versione OpenMP.

---

## Analisi dei Risultati
1. **OpenMP**: La versione OpenMP migliora significativamente rispetto alla seriale per un numero maggiore di corpi (fino a **70% di miglioramento** con 1024 corpi).
2. **MPI + OpenMP**:
   - La combinazione di MPI e OpenMP raggiunge il miglioramento massimo rispetto alla seriale con **78.39% di riduzione del tempo** per 1024 corpi.
   - Rispetto a OpenMP, MPI + OpenMP diventa più vantaggiosa per un numero elevato di corpi, con un miglioramento fino al **26.11%**.
3. **Svantaggi per pochi corpi**: Per un numero ridotto di corpi, MPI + OpenMP ha un sovraccarico maggiore rispetto a OpenMP puro, ma diventa più efficiente con carichi più alti.

---
