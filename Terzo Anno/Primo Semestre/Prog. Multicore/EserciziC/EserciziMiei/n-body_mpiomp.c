#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

// Costante gravitazionale (arbitraria per semplificazione)
#define G 6.67430e-11

// Struttura per rappresentare un corpo
typedef struct {
    double x, y, z;    // Posizione
    double vx, vy, vz; // Velocità
    double mass;       // Massa
} Body;

// Funzione per calcolare le forze gravitazionali e aggiornare velocità e posizione
void update_bodies(Body *local_bodies, int local_n, Body *all_bodies, int total_n, double dt) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < local_n; i++) {
        double fx = 0, fy = 0, fz = 0;
        for (int j = 0; j < total_n; j++) {
            if (i != j) {
                double dx = all_bodies[j].x - local_bodies[i].x;
                double dy = all_bodies[j].y - local_bodies[i].y;
                double dz = all_bodies[j].z - local_bodies[i].z;
                double dist = sqrt(dx * dx + dy * dy + dz * dz);
                if (dist > 1e-6) { // Evita divisioni per zero
                    double force = G * local_bodies[i].mass * all_bodies[j].mass / (dist * dist);
                    fx += force * dx / dist;
                    fy += force * dy / dist;
                    fz += force * dz / dist;
                }
            }
        }
        // Aggiorna le velocità
        local_bodies[i].vx += fx / local_bodies[i].mass * dt;
        local_bodies[i].vy += fy / local_bodies[i].mass * dt;
        local_bodies[i].vz += fz / local_bodies[i].mass * dt;
    }

    // Aggiorna le posizioni
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < local_n; i++) {
        local_bodies[i].x += local_bodies[i].vx * dt;
        local_bodies[i].y += local_bodies[i].vy * dt;
        local_bodies[i].z += local_bodies[i].vz * dt;
    }
}

// Funzione per inizializzare i corpi con valori casuali
void initialize_bodies(Body *bodies, int n) {
    for (int i = 0; i < n; i++) {
        bodies[i].x = rand() % 1000;
        bodies[i].y = rand() % 1000;
        bodies[i].z = rand() % 1000;
        bodies[i].vx = bodies[i].vy = bodies[i].vz = 0;
        bodies[i].mass = rand() % 100 + 1; // Massa casuale tra 1 e 100
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 3) {
        if (rank == 0) {
            printf("Usage: %s <number_of_bodies> <number_of_threads>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);       // Numero totale di corpi
    int num_threads = atoi(argv[2]); // Numero di thread per processo
    omp_set_num_threads(num_threads);

    int local_n = n / size; // Numero di corpi per processo
    Body *local_bodies = (Body *)malloc(local_n * sizeof(Body));
    Body *all_bodies = (Body *)malloc(n * sizeof(Body));

    if (rank == 0) {
        // Processo master inizializza tutti i corpi
        initialize_bodies(all_bodies, n);
    }

    // Broadcast di tutti i corpi a tutti i processi
    MPI_Bcast(all_bodies, n * sizeof(Body), MPI_BYTE, 0, MPI_COMM_WORLD);

    // Suddivisione dei corpi tra i processi
    MPI_Scatter(all_bodies, local_n * sizeof(Body), MPI_BYTE, local_bodies, local_n * sizeof(Body), MPI_BYTE, 0, MPI_COMM_WORLD);

    double dt = 0.01;  // Intervallo di tempo
    int steps = 100;   // Numero di passi temporali
    double start_time = MPI_Wtime();

    for (int step = 0; step < steps; step++) {
        // Aggiorna le forze e le posizioni per i corpi locali
        update_bodies(local_bodies, local_n, all_bodies, n, dt);

        // Aggiorna la posizione di tutti i corpi attraverso tutti i processi
        MPI_Allgather(local_bodies, local_n * sizeof(Body), MPI_BYTE, all_bodies, local_n * sizeof(Body), MPI_BYTE, MPI_COMM_WORLD);
    }

    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;

    // Il processo master calcola il tempo totale
    if (rank == 0) {
        printf("Total execution time for %d bodies with %d processes and %d threads per process: %.6f seconds\n", n, size, num_threads, elapsed_time);
    }

    free(local_bodies);
    free(all_bodies);

    MPI_Finalize();
    return 0;
}
