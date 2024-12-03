#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 100  // Valore massimo per gli elementi dell'array

typedef struct {
    int *array;
    int start;
    int end;
    int phase;
    int n;
} thread_data_t;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Funzione eseguita da ogni thread
void *odd_even_sort(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int *array = data->array;
    int start = data->start;
    int end = data->end;
    int phase = data->phase;

    for (int i = phase; i < data->n - 1; i += 2) {
        for (int j = start; j < end - 1; j += 2) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
    pthread_exit(NULL);
}

void print_array(int *array, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <numero di elementi> <numero di thread>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    if (num_threads > n) num_threads = n;

    int *array = malloc(n * sizeof(int));
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    // Inizializza array con valori casuali
    srand(time(NULL));
    printf("Array non ordinato: ");
    for (int i = 0; i < n; i++) {
        array[i] = rand() % MAX;
        printf("%d ", array[i]);
    }
    printf("\n");

    // Odd-even sort multithread
    for (int phase = 0; phase < n; phase++) {
        int chunk_size = (n + num_threads - 1) / num_threads;

        for (int t = 0; t < num_threads; t++) {
            thread_data[t].array = array;
            thread_data[t].start = t * chunk_size;
            thread_data[t].end = (t + 1) * chunk_size > n ? n : (t + 1) * chunk_size;
            thread_data[t].phase = phase % 2;

            pthread_create(&threads[t], NULL, odd_even_sort, &thread_data[t]);
        }

        for (int t = 0; t < num_threads; t++) {
            pthread_join(threads[t], NULL);
        }
    }

    // Stampa l'array ordinato
    printf("Array ordinato: ");
    print_array(array, n);

    free(array);
    return 0;
}
