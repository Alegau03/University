#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Funzione di scambio
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Funzione di partizionamento per Quicksort
int partition(int *array, int low, int high) {
    int pivot = array[high];  // Scegliamo l'ultimo elemento come pivot
    int i = (low - 1);  // Indice del più piccolo elemento

    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;  // Incrementa l'indice del più piccolo elemento
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

// Funzione di ordinamento locale con Quicksort
void quicksort(int *array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);

        // Ordina gli elementi separatamente prima e dopo la partizione
        quicksort(array, low, pi - 1);
        quicksort(array, pi + 1, high);
    }
}

// Funzione per calcolare il partner con cui scambiare
int compute_partner(int phase, int my_rank, int comm_sz) {
    if (phase % 2 == 0) {  // Fasi pari
        if (my_rank % 2 == 0 && my_rank + 1 < comm_sz) {
            return my_rank + 1;
        } else if (my_rank % 2 == 1) {
            return my_rank - 1;
        }
    } else {  // Fasi dispari
        if (my_rank % 2 == 1 && my_rank + 1 < comm_sz) {
            return my_rank + 1;
        } else if (my_rank % 2 == 0) {
            return my_rank - 1;
        }
    }
    return -1;  // Se non c'è partner valido
}

// Funzione principale
int main(int argc, char *argv[]) {
    int comm_sz, my_rank;
    int *local_array, *received_array;
    int local_n, n;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Controllo se il numero di argomenti è corretto
    if (argc != 2) {
        if (my_rank == 0) {
            fprintf(stderr, "Uso: %s <numero di elementi>\n", argv[0]);
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // Leggo il numero di elementi da ordinare da argv
    n = atoi(argv[1]);
    
    // Assicuro che n sia divisibile per il numero di processi
    if (n % comm_sz != 0) {
        if (my_rank == 0) {
            fprintf(stderr, "Il numero di elementi deve essere divisibile per il numero di processi.\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    local_n = n / comm_sz;
    local_array = malloc(local_n * sizeof(int));
    received_array = malloc(local_n * sizeof(int));

    // Genero un array casuale da ordinare nel processo 0 e lo stampo
    if (my_rank == 0) {
        int *array = malloc(n * sizeof(int));
        printf("Array non ordinato: ");
        for (int i = 0; i < n; i++) {
            array[i] = rand() % 100;  // Genera numeri casuali
            printf("%d ", array[i]);
        }
        printf("\n");

        // Divido l'array tra i processi
        MPI_Scatter(array, local_n, MPI_INT, local_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        free(array);
    } else {
        MPI_Scatter(NULL, local_n, MPI_INT, local_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Ogni processo ordina localmente il proprio sottoarray con Quicksort
    quicksort(local_array, 0, local_n - 1);

    // Fasi dell'Odd-even transposition sort
    for (int phase = 0; phase < comm_sz; phase++) {
        int partner = compute_partner(phase, my_rank, comm_sz);

        if (partner >= 0 && partner < comm_sz) {
            MPI_Sendrecv(local_array, local_n, MPI_INT, partner, 0, 
                         received_array, local_n, MPI_INT, partner, 0, 
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (my_rank < partner) {
                // Conservo i numeri più piccoli
                for (int i = 0; i < local_n; i++) {
                    if (local_array[i] > received_array[i]) {
                        swap(&local_array[i], &received_array[i]);
                    }
                }
            } else {
                // Conservo i numeri più grandi
                for (int i = 0; i < local_n; i++) {
                    if (local_array[i] < received_array[i]) {
                        swap(&local_array[i], &received_array[i]);
                    }
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);  // Sincronizzo i processi alla fine di ogni fase
    }

    // Raccolgo i risultati ordinati nel processo 0
    int *sorted_array = NULL;
    if (my_rank == 0) {
        sorted_array = malloc(n * sizeof(int));
    }

    MPI_Gather(local_array, local_n, MPI_INT, sorted_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Stampo l'array ordinato nel processo 0
    if (my_rank == 0) {
        printf("Array ordinato: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", sorted_array[i]);
        }
        printf("\n");
        free(sorted_array);
    }

    free(local_array);
    free(received_array);
    MPI_Finalize();
    return 0;
}
