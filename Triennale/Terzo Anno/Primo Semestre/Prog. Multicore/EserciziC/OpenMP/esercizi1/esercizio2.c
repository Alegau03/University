#include <stdio.h>
#include <stdlib.h>
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

void matrix_multiply_parallel(double *A, double *B, double *C, int n, int m, int p, int num_threads) {
    // Imposta il numero di thread OpenMP
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

void matrix_multiply_sequential(double *A, double *B, double *C, int n, int m, int p) {
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
    if (argc != 5) {
        printf("Usage: %s <n> <m> <p> <num_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]); // Rows of A
    int m = atoi(argv[2]); // Columns of A and rows of B
    int p = atoi(argv[3]); // Columns of B
    int num_threads = atoi(argv[4]); // Number of threads

    // Allocate memory for matrices
    double *A = (double *)malloc(n * m * sizeof(double));
    double *B = (double *)malloc(m * p * sizeof(double));
    double *C_parallel = (double *)malloc(n * p * sizeof(double));
    double *C_sequential = (double *)malloc(n * p * sizeof(double));

    // Generate random matrices
    srand(time(NULL));
    for (int i = 0; i < n * m; i++) {
        A[i] = (rand() % 10) + 1; // Random values between 1 and 10
    }
    for (int i = 0; i < m * p; i++) {
        B[i] = (rand() % 10) + 1;
    }

    // Print input matrices
    print_matrix("Matrix A", A, n, m);
    print_matrix("Matrix B", B, m, p);

    // Parallel computation
    double start_parallel = omp_get_wtime();
    matrix_multiply_parallel(A, B, C_parallel, n, m, p, num_threads);
    double end_parallel = omp_get_wtime();

    // Print result matrix from parallel computation
    print_matrix("Matrix C (Parallel)", C_parallel, n, p);

    // Sequential computation for validation
    double start_sequential = omp_get_wtime();
    matrix_multiply_sequential(A, B, C_sequential, n, m, p);
    double end_sequential = omp_get_wtime();

    // Print result matrix from sequential computation
    print_matrix("Matrix C (Sequential)", C_sequential, n, p);

    // Verify results
    int correct = 1;
    for (int i = 0; i < n * p; i++) {
        if (C_parallel[i] != C_sequential[i]) {
            correct = 0;
            break;
        }
    }

    if (correct) {
        printf("The result is correct!\n");
    } else {
        printf("The result is incorrect!\n");
    }

    // Print timing information
    printf("Parallel execution time: %.6f seconds\n", end_parallel - start_parallel);
    printf("Sequential execution time: %.6f seconds\n", end_sequential - start_sequential);

    // Free allocated memory
    free(A);
    free(B);
    free(C_parallel);
    free(C_sequential);

    return 0;
}
