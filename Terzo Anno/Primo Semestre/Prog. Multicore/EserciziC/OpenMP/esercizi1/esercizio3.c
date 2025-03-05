// Versione con OpenMP

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <time.h>

// Funzione per stampare una matrice
void print_matrix(const char *name, double *matrix, int rows, int cols) {
    printf("%s (%dx%d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6.2f ", matrix[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Moltiplicazione parallela con OpenMP
void matrix_multiply_openmp(double *A, double *B, double *C, int n, int m, int p, int num_threads) {
    omp_set_num_threads(num_threads);
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            C[i * p + j] = 0.0;
            for (int k = 0; k < m; k++) {
                C[i * p + j] += A[i * m + k] * B[k * p + j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 5) {
        if (rank == 0) {
            printf("Usage: %s <n> <m> <p> <num_threads>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]); // Rows of A
    int m = atoi(argv[2]); // Columns of A and rows of B
    int p = atoi(argv[3]); // Columns of B
    int num_threads = atoi(argv[4]); // Processi MPI
    

    double *A = NULL, *B = NULL, *C = NULL; // Matrici globali
    double *A_local, *C_local; // Partizioni locali

    int rows_per_proc = n / size;
    if (rank == 0) {
        // Generazione delle matrici
        A = (double *)malloc(n * m * sizeof(double));
        B = (double *)malloc(m * p * sizeof(double));
        C = (double *)malloc(n * p * sizeof(double));
        srand(time(NULL));
        for (int i = 0; i < n * m; i++) {
            A[i] = (rand() % 10) + 1;
        }
        for (int i = 0; i < m * p; i++) {
            B[i] = (rand() % 10) + 1;
        }
    }

    // Allocazione locale
    A_local = (double *)malloc(rows_per_proc * m * sizeof(double));
    C_local = (double *)malloc(rows_per_proc * p * sizeof(double));
    if (rank == 0) {
        //print_matrix("Matrix A", A, n, m);
        //print_matrix("Matrix B", B, m, p);
    }

    // Broadcast di B a tutti i processi
    if (rank == 0) {
        MPI_Bcast(B, m * p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        B = (double *)malloc(m * p * sizeof(double));
        MPI_Bcast(B, m * p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Scatter di A tra i processi
    MPI_Scatter(A, rows_per_proc * m, MPI_DOUBLE, A_local, rows_per_proc * m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Moltiplicazione parallela locale con OpenMP
    double start = MPI_Wtime();
    matrix_multiply_openmp(A_local, B, C_local, rows_per_proc, m, p, num_threads);
    double end = MPI_Wtime();

    // Gather dei risultati in rank 0
    MPI_Gather(C_local, rows_per_proc * p, MPI_DOUBLE, C, rows_per_proc * p, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        //print_matrix("Matrix C", C, n, p);
        printf("Execution Time: %.6f seconds\n", end - start);
    }

    // Libera memoria
    free(A_local);
    free(C_local);
    if (rank == 0) {
        free(A);
        free(B);
        free(C);
    } else {
        free(B);
    }

    MPI_Finalize();
    return 0;
}
