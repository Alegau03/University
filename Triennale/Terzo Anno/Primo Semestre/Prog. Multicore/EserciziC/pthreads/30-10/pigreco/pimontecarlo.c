#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Numero totale di lanci e numero di lanci nel cerchio
long total_tosses;
long number_in_circle = 0;

// Mutex per sincronizzare l'accesso alla variabile condivisa number_in_circle
pthread_mutex_t mutex;

// Funzione che ogni thread eseguirà per calcolare i lanci
void* toss_darts(void* arg) {
    long local_tosses = *(long*)arg; // Numero di lanci assegnati al thread
    long local_in_circle = 0; // Conta dei lanci nel cerchio per questo thread

    for (long i = 0; i < local_tosses; i++) {
        // Genera coordinate casuali tra -1 e 1
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;

        // Calcola la distanza dall'origine
        double distance_squared = x * x + y * y;

        // Verifica se il lancio è nel cerchio
        if (distance_squared <= 1) {
            local_in_circle++;
        }
    }

    // Sezione critica per aggiornare il conteggio globale
    pthread_mutex_lock(&mutex);
    number_in_circle += local_in_circle;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Utilizzo: %s <numero_di_lanci> <numero_di_thread>\n", argv[0]);
        return -1;
    }

    total_tosses = atol(argv[1]); // Numero totale di lanci
    int thread_count = atoi(argv[2]); // Numero di thread da creare

    pthread_t threads[thread_count];
    pthread_mutex_init(&mutex, NULL); // Inizializza il mutex

    // Calcola il numero di lanci per ogni thread
    long tosses_per_thread = total_tosses / thread_count;

    // Crea i thread e assegna loro una parte dei lanci
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], NULL, toss_darts, &tosses_per_thread);
    }

    // Aspetta che tutti i thread finiscano
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calcola la stima di π
    double pi_estimate = 4.0 * number_in_circle / total_tosses;

    printf("Stima di Pi: %f\n", pi_estimate);

    // Distrugge il mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
