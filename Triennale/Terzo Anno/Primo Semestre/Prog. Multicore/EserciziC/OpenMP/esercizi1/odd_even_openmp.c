#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MAX 100  // Valore massimo per gli elementi dell'array

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void print_array(int *array, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void odd_even_sort(int *array, int n, int num_threads) {
    omp_set_num_threads(num_threads);

    for (int phase = 0; phase < n; phase++) {
        #pragma omp parallel for
        for (int i = (phase % 2); i < n - 1; i += 2) {
            if (array[i] > array[i + 1]) {
                swap(&array[i], &array[i + 1]);
            }
        }
    }
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

    // Inizializza array con valori casuali
    srand(time(NULL));
    printf("Array non ordinato: ");
    for (int i = 0; i < n; i++) {
        array[i] = rand() % MAX;
        printf("%d ", array[i]);
    }
    printf("\n");

    // Odd-even sort con OpenMP
    odd_even_sort(array, n, num_threads);

    // Stampa l'array ordinato
    printf("Array ordinato: ");
    print_array(array, n);

    free(array);
    return 0;
}
