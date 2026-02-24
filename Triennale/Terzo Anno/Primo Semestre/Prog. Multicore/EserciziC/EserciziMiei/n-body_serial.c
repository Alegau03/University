#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Costante gravitazionale (arbitraria per semplificazione)
#define G 6.67430e-11

// Struttura per rappresentare un corpo
typedef struct {
    double x, y, z;    // Posizione
    double vx, vy, vz; // Velocità
    double mass;       // Massa
} Body;

// Funzione per calcolare le forze gravitazionali e aggiornare velocità e posizione
void update_bodies(Body *bodies, int n, double dt) {
    for (int i = 0; i < n; i++) {
        double fx = 0, fy = 0, fz = 0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double dx = bodies[j].x - bodies[i].x;
                double dy = bodies[j].y - bodies[i].y;
                double dz = bodies[j].z - bodies[i].z;
                double dist = sqrt(dx * dx + dy * dy + dz * dz);
                if (dist > 1e-6) { // Evita divisioni per zero
                    double force = G * bodies[i].mass * bodies[j].mass / (dist * dist);
                    fx += force * dx / dist;
                    fy += force * dy / dist;
                    fz += force * dz / dist;
                }
            }
        }
        // Aggiorna le velocità
        bodies[i].vx += fx / bodies[i].mass * dt;
        bodies[i].vy += fy / bodies[i].mass * dt;
        bodies[i].vz += fz / bodies[i].mass * dt;
    }

    // Aggiorna le posizioni
    for (int i = 0; i < n; i++) {
        bodies[i].x += bodies[i].vx * dt;
        bodies[i].y += bodies[i].vy * dt;
        bodies[i].z += bodies[i].vz * dt;
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
    if (argc != 2) {
        printf("Usage: %s <number_of_bodies>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]); // Numero di corpi
    if (n <= 0) {
        printf("Number of bodies must be greater than 0.\n");
        return 1;
    }

    Body *bodies = (Body *)malloc(n * sizeof(Body));
    if (!bodies) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    initialize_bodies(bodies, n);

    int num_iterations = 100;  // Numero di esecuzioni per benchmark
    double dt = 0.01;          // Intervallo di tempo
    int steps = 100;           // Numero di passi temporali
    double total_time = 0.0;

    for (int iter = 0; iter < num_iterations; iter++) {
        clock_t start = clock();

        for (int step = 0; step < steps; step++) {
            update_bodies(bodies, n, dt);
        }

        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }

    double avg_time = total_time / num_iterations;
    printf("Average execution time for %d bodies over %d iterations: %.6f seconds\n", n, num_iterations, avg_time);

    free(bodies);
    return 0;
}
