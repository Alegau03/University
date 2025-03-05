#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double* alloc_matrix(int rows, int cols) {
    return (double*)malloc(rows * cols * sizeof(double));
}

void generate_random_matrix(double* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = (double)(rand() % 10);
        }
    }
}

void print_matrix(double* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6.2f ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

void free_matrix(double* matrix) {
    free(matrix);
}

void update_matrix(double* old_matrix, double* new_matrix, int rows, int cols) {
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            double sum = old_matrix[(i - 1) * cols + j] // Nord
                       + old_matrix[i * cols + (j - 1)] // Ovest
                       + old_matrix[(i + 1) * cols + j] // Sud
                       + old_matrix[i * cols + (j + 1)]; // Est
            new_matrix[i * cols + j] = sum;
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    int rows, cols, iterations;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 4) {
        if (rank == 0) {
            printf("Usage: %s <rows> <cols> <iterations>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    rows = atoi(argv[1]);
    cols = atoi(argv[2]);
    iterations = atoi(argv[3]);

    double* matrix = NULL;
    if (rank == 0) {
        matrix = alloc_matrix(rows, cols);
        generate_random_matrix(matrix, rows, cols);
        printf("Matrice A iniziale:\n");
        print_matrix(matrix, rows, cols);
    }

    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_rows = rows / size + 2; // aggiungi due righe per i bordi
    double* local_matrix = alloc_matrix(local_rows, cols);
    double* new_matrix = alloc_matrix(local_rows, cols);

    MPI_Scatter(matrix, (rows / size) * cols, MPI_DOUBLE, &local_matrix[cols], (rows / size) * cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int iter = 0; iter < iterations; iter++) {
        // Scambio delle righe di confine
        if (rank > 0) {
            MPI_Sendrecv(&local_matrix[cols], cols, MPI_DOUBLE, rank - 1, 0,
                         &local_matrix[0], cols, MPI_DOUBLE, rank - 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank < size - 1) {
            MPI_Sendrecv(&local_matrix[(local_rows - 2) * cols], cols, MPI_DOUBLE, rank + 1, 0,
                         &local_matrix[(local_rows - 1) * cols], cols, MPI_DOUBLE, rank + 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        update_matrix(local_matrix, new_matrix, local_rows, cols);

        double* temp = local_matrix;
        local_matrix = new_matrix;
        new_matrix = temp;

        MPI_Gather(&local_matrix[cols], (rows / size) * cols, MPI_DOUBLE, matrix, (rows / size) * cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            printf("Matrice A dopo l'operazione %d:\n", iter + 1);
            print_matrix(matrix, rows, cols);
        }
    }

    free_matrix(local_matrix);
    free_matrix(new_matrix);
    if (rank == 0) {
        free_matrix(matrix);
    }

    MPI_Finalize();
    return 0;
}
