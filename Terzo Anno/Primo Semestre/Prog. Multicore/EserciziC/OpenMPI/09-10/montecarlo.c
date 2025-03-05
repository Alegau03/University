#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    int rank, size;
    long long int total_tosses = 1000000;  // Default number of tosses
    long long int tosses_per_process;
    long long int number_in_circle = 0;
    long long int total_in_circle = 0;
    
    MPI_Init(&argc, &argv);  // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total number of processes

    if (rank == 0) {
        // Master process (rank 0) gets the number of tosses from the user
        if (argc == 2) {
            total_tosses = atoll(argv[1]);  // tosses 
        }
        printf("Total tosses: %lld\n", total_tosses);
    }

    // Broadcast the total number of tosses to all processes
    MPI_Bcast(&total_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    // Each process will handle an equal share of tosses
    tosses_per_process = total_tosses / size;

    // Initialize random number generator with different seed for each process
    srand(time(NULL) + rank);

    // Monte Carlo simulation: each process simulates tosses_per_process tosses
    for (long long int toss = 0; toss < tosses_per_process; toss++) {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;  // Random double between -1 and 1
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;  // Random double between -1 and 1
        double distance_squared = x * x + y * y;

        if (distance_squared <= 1.0) {
            number_in_circle++;
        }
    }

    // Reduce all the number_in_circle from all processes to total_in_circle
    MPI_Reduce(&number_in_circle, &total_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Master process calculates the estimate of pi
    if (rank == 0) {
        double pi_estimate = 4.0 * (double)total_in_circle / (double)total_tosses;
        printf("Estimated value of Pi: %f\n", pi_estimate);
    }

    MPI_Finalize();  // Finalize MPI
    return 0;
}
