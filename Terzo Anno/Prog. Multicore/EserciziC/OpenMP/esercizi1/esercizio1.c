#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int row_start;
    int row_end;
    int n;
    int m;
    int p;
    double *A;
    double *B;
    double *C;
} ThreadData;

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

void *matrix_multiply_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int row_start = data->row_start;
    int row_end = data->row_end;
    int n = data->n;
    int m = data->m;
    int p = data->p;
    double *A = data->A;
    double *B = data->B;
    double *C = data->C;

    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < p; j++) {
            C[i * p + j] = 0.0;
            for (int k = 0; k < m; k++) {
                C[i * p + j] += A[i * m + k] * B[k * p + j];
            }
        }
    }

    return NULL;
}

void sequential_multiply(double *A, double *B, double *C, int n, int m, int p) {
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
    int num_threads = atoi(argv[4]);

    // Allocate memory for matrices
    double *A = (double *)malloc(n * m * sizeof(double));
    double *B = (double *)malloc(m * p * sizeof(double));
    double *C = (double *)malloc(n * p * sizeof(double));
    double *C_seq = (double *)malloc(n * p * sizeof(double));

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

    // Create threads and distribute work
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int rows_per_thread = n / num_threads;
    int extra_rows = n % num_threads;

    for (int t = 0; t < num_threads; t++) {
        thread_data[t].row_start = t * rows_per_thread + (t < extra_rows ? t : extra_rows);
        thread_data[t].row_end = thread_data[t].row_start + rows_per_thread + (t < extra_rows);
        thread_data[t].n = n;
        thread_data[t].m = m;
        thread_data[t].p = p;
        thread_data[t].A = A;
        thread_data[t].B = B;
        thread_data[t].C = C;
        pthread_create(&threads[t], NULL, matrix_multiply_thread, &thread_data[t]);
    }

    // Wait for threads to finish
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    // Print result matrix from parallel computation
    print_matrix("Matrix C (Parallel)", C, n, p);

    // Sequential computation for validation
    sequential_multiply(A, B, C_seq, n, m, p);

    // Print result matrix from sequential computation
    print_matrix("Matrix C (Sequential)", C_seq, n, p);

    // Verify results
    int correct = 1;
    for (int i = 0; i < n * p; i++) {
        if (C[i] != C_seq[i]) {
            correct = 0;
            break;
        }
    }

    if (correct) {
        printf("The result is correct!\n");
    } else {
        printf("The result is incorrect!\n");
    }

    // Free allocated memory
    free(A);
    free(B);
    free(C);
    free(C_seq);

    return 0;
}
