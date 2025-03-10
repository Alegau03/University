#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "my_timer.h"

double x[MAX];   

int compare(const void* a, const void* b) {
   return (*(double*)a - *(double*)b);
}

// Computes matrix-vector multiplication sequentially
int main(int argc, char** argv) {
    int i,iter;
    srand(time(NULL));
    for (i = 0; i < MAX; i++) {
        x[i] = rand() % 10;
    }
    qsort(x, MAX, sizeof(double), compare);

    double total_time = 0.0;
    int total_smaller = 0;
    for(iter = 0; iter < ITER; iter++){
        double start, stop;
        int smaller = 0;
        GET_TIME(start);
        for(i = 0; i < MAX; i++){
            if(x[i] < 5){
                smaller++;
            }
        }
        GET_TIME(stop);
        total_smaller += smaller;
        total_time += stop-start;
    }
    printf("Total smaller %d\n", total_smaller);
    printf("Average runtime %f usec\n", total_time/ITER);
}
