#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double get_neighbor_sum(double **matrix, int layer, int row, int col, int layers, int rows, int cols) {
    double sum = 0.0;
    int directions[6][3] = {
        {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}, {1, 0, 0}, {-1, 0, 0}
    };

    for (int i = 0; i < 6; i++) {
        int new_layer = layer + directions[i][0];
        int new_row = row + directions[i][1];
        int new_col = col + directions[i][2];

        if (new_layer >= 0 && new_layer < layers && new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols) {
            sum += matrix[new_layer][new_row * cols + new_col];  // Accesso come bidimensionale
        }
    }
    return sum;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int rows, cols, layers, iterations;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 5) {
        if (rank == 0) {
            printf("Usage: %s <rows> <cols> <layers> <iterations>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    rows = atoi(argv[1]);
    cols = atoi(argv[2]);
    layers = atoi(argv[3]);
    iterations = atoi(argv[4]);

    int layers_per_process = layers / size;
    int remaining_layers = layers % size;

    if (rank < remaining_layers) {
        layers_per_process++;
    }

    double **sub_matrix = (double **)malloc((layers_per_process + 2) * sizeof(double *));
    double **new_matrix = (double **)malloc((layers_per_process + 2) * sizeof(double *));
    
    for (int i = 0; i < layers_per_process + 2; i++) {
        sub_matrix[i] = (double *)malloc(rows * cols * sizeof(double));
        new_matrix[i] = (double *)malloc(rows * cols * sizeof(double));
    }

    for (int i = 0; i < layers_per_process + 2; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                sub_matrix[i][j * cols + k] = (double)(rand() % 10); // Inizializza con valori casuali
                new_matrix[i][j * cols + k] = 0.0; // Inizializza la nuova matrice
            }
        }
    }

    // Stampa la matrice iniziale
    for (int i = 1; i <= layers_per_process; i++) {
        printf("Layer %d (Process %d):\n", rank * layers_per_process + i, rank);
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                printf("%5.2f ", sub_matrix[i][j * cols + k]);
            }
            printf("\n");
        }
    }

    // Iterazioni di aggiornamento
    for (int it = 0; it < iterations; it++) {
        for (int i = 1; i <= layers_per_process; i++) {
            for (int j = 0; j < rows; j++) {
                for (int k = 0; k < cols; k++) {
                    new_matrix[i][j * cols + k] = get_neighbor_sum(sub_matrix, i, j, k, layers_per_process, rows, cols);
                }
            }
        }

        // Scambio delle matrici (buffer di input/output separati)
        double **temp = sub_matrix;
        sub_matrix = new_matrix;
        new_matrix = temp;
    }

    // Stampa la matrice finale
    for (int i = 1; i <= layers_per_process; i++) {
        printf("Layer %d after iterations (Process %d):\n", rank * layers_per_process + i, rank);
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                printf("%5.2f ", sub_matrix[i][j * cols + k]);
            }
            printf("\n");
        }
    }

    for (int i = 0; i < layers_per_process + 2; i++) {
        free(sub_matrix[i]);
        free(new_matrix[i]);
    }
    free(sub_matrix);
    free(new_matrix);

    MPI_Finalize();
    return 0;
}
