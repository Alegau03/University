#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Funzioni per gestire le matrici
double* alloc_matrix(int rows, int cols) {
    return (double*)malloc(rows * cols * sizeof(double));
}
//Genera la matrice random
void generate_random_matrix(double* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = (double)(rand() % 10);
        }
    }
}

//Stampa la matrice
void print_matrix(double* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6.2f ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

//Libera la memoria della matrice
void free_matrix(double* matrix) {
    free(matrix);
}

// Moltiplicazione sequenziale tra matrici (per controllo)
void sequential_matrix_multiply(double* A, double* B, double* C, int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            C[i * colsB + j] = 0.0;
            for (int k = 0; k < colsA; k++) {
                C[i * colsB + j] += A[i * colsA + k] * B[k * colsB + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 5) {
        if (rank == 0) {
            printf("Utilizzo: %s <righeA> <colonneA> <righeB> <colonneB>\n", argv[0]);
        }
        MPI_Finalize();
        return -1;
    }

    int rowsA = atoi(argv[1]);
    int colsA = atoi(argv[2]);
    int rowsB = atoi(argv[3]);
    int colsB = atoi(argv[4]);

    if (colsA != rowsB) {
        if (rank == 0) {
            printf("Errore: colsA deve essere uguale a rowsB.\n");
        }
        MPI_Finalize();
        return -1;
    }

    double* A_flat = NULL;
    double* B_flat = NULL;
    double* C_flat = NULL;

    if (rank == 0) {
        A_flat = alloc_matrix(rowsA, colsA);
        B_flat = alloc_matrix(rowsB, colsB);
        generate_random_matrix(A_flat, rowsA, colsA);
        generate_random_matrix(B_flat, rowsB, colsB);

        printf("Matrice A:\n");
        print_matrix(A_flat, rowsA, colsA);
        printf("Matrice B:\n");
        print_matrix(B_flat, rowsB, colsB);
    }


    int rows_per_process = rowsA / size; //righe per processo
    int remainder = rowsA % size; //righe in più date ai primi processi

    // Calcola il numero di righe assegnate a ciascun processo
    int* recvcounts = malloc(size * sizeof(int));
    int* displs = malloc(size * sizeof(int));
    int offset = 0;

    for (int i = 0; i < size; i++) {
        recvcounts[i] = (i < remainder) ? (rows_per_process + 1) * colsA : rows_per_process * colsA; //calcola ogni processo i quante righe prenderà, se è tra i primi prende una riga in più 
        displs[i] = offset;  //Da che punto di A_flat può leggere le righe 
        offset += recvcounts[i]; //aggiorna l'offset
    }

    double* A_local = alloc_matrix(recvcounts[rank] / colsA, colsA); //alloca matrice A LOCALE PER OGNI PROCESSO
    
    // Scatterv per la matrice A
    MPI_Scatterv(A_flat, recvcounts, displs, MPI_DOUBLE, A_local, recvcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); //Ogni processo riceve la sua porzione della matrice 
                                                                                                                    // Originale A_flat e la mette in A_local
    // Broadcast della matrice B a tutti i processi
    if (rank != 0) {
        B_flat = alloc_matrix(rowsB, colsB);
    }
    MPI_Bcast(B_flat, rowsB * colsB, MPI_DOUBLE, 0, MPI_COMM_WORLD); //mando la matrice B a tutti

    // Calcola la matrice C locale
    double* C_local = alloc_matrix(recvcounts[rank] / colsA, colsB);
    

    //Operazione di calcolo prodotto tra matrici
    for (int i = 0; i < recvcounts[rank] / colsA; i++) {
        for (int j = 0; j < colsB; j++) {
            C_local[i * colsB + j] = 0.0;
            for (int k = 0; k < colsA; k++) {
                C_local[i * colsB + j] += A_local[i * colsA + k] * B_flat[k * colsB + j];
            }
        }
    }

    // Solo il processo 0 raccoglie i dati
    if (rank == 0) {
        C_flat = alloc_matrix(rowsA, colsB);
    }

    // Gatherv per raccogliere i risultati, aggiorno i dati di recvcounts e displs aggiornati per adattarmi alla matrice C
    for (int i = 0; i < size; i++) {
        recvcounts[i] = (recvcounts[i] / colsA) * colsB;
        displs[i] = (displs[i] / colsA) * colsB;
    }
    
    MPI_Gatherv(C_local, recvcounts[rank], MPI_DOUBLE, C_flat, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Stampa la matrice C finale solo per il processo 0
    if (rank == 0) {
        printf("Matrice C (Risultato):\n");
        print_matrix(C_flat, rowsA, colsB);

        // Verifica del risultato
        double* C_sequential = alloc_matrix(rowsA, colsB);
        sequential_matrix_multiply(A_flat, B_flat, C_sequential, rowsA, colsA, colsB);
        printf("Matrice C (Risultato sequenziale):\n");
        print_matrix(C_sequential, rowsA, colsB);
        // Controlla se i risultati sono uguali
        int correct = 1;
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsB; j++) {
                if (C_flat[i * colsB + j] != C_sequential[i * colsB + j]) {
                    correct = 0;
                    break;
                }
            }
        }

        if (correct) {
            printf("I risultati sono corretti!\n");
        } else {
            printf("I risultati sono errati!\n");
        }

        free_matrix(C_sequential);
    }

    // Pulizia della memoria
    free_matrix(A_flat);
    free_matrix(B_flat);
    free_matrix(A_local);
    free_matrix(C_local);
    free_matrix(C_flat);
    free(recvcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}
