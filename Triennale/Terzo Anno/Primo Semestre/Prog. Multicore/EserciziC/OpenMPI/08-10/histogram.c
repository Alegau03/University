#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_SIZE 100   // Size of the input data
#define BINS 10         // Number of histogram bins

// Function to compute histogram locally
void compute_local_histogram(int *data, int data_size, int *local_histogram, int bins, int min_value, int max_value) {
    int bin_width = (max_value - min_value) / bins;
    for (int i = 0; i < data_size; i++) {
        int bin_index = (data[i] - min_value) / bin_width;
        if (bin_index >= bins) {
            bin_index = bins - 1;
        }
        local_histogram[bin_index]++;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int data[DATA_SIZE];       // Input data array
    int global_histogram[BINS] = {0}; // Global histogram
    int local_histogram[BINS] = {0};  // Local histogram for each process
    int local_data_size;       // Size of the data each process handles
    int *local_data = NULL;
    int min_value = 0, max_value = 100;  

    MPI_Init(&argc, &argv);              // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get number of processes

    local_data_size = DATA_SIZE / size;  // Divide data among processes

    // Process 0 generates input data
    if (rank == 0) {
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i] = rand() % (max_value - min_value + 1) + min_value;  // Random data between min_value and max_value
        }
    }

    // Allocate memory for local data
    local_data = (int *)malloc(local_data_size * sizeof(int));

    // Scatter data from process 0 to all processes
    MPI_Scatter(data, local_data_size, MPI_INT, local_data, local_data_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes its local histogram
    compute_local_histogram(local_data, local_data_size, local_histogram, BINS, min_value, max_value);

    // Gather local histograms to the global histogram in process 0
    MPI_Reduce(local_histogram, global_histogram, BINS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Process 0 prints the global histogram
    if (rank == 0) {
        printf("Final Histogram:\n");
        for (int i = 0; i < BINS; i++) {
            printf("Bin %d: %d\n", i, global_histogram[i]);
        }
    }

    free(local_data);
    MPI_Finalize();  // Finalize MPI
    return 0;
}
    