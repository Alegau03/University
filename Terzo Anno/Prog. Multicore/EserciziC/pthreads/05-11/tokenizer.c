#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// QUESTA FUNZIONE USA STRKTOK CHE NON FUNZIONA CON PIù THREADS!


int thread_count;

void *Tokenize(void * rank){

    long my_rank= (long) rank;
    int count;
    int next = (my_rank+1)%thread_count;
    char *fg_rv;
    char my_link[100];
    char *my_string;

    sem_wait(&sems[my_rank]);
    fg_rv = fgets(my_line, 100, stdin);
    sem_post(&sems[next]);
    while(fg_rv!=NULL){
        printf("Thread %ld > my line = %s", my_rank, my_line);
        count = 0;
        my_string = strtok(my_line, " \t\n");
        while(my_string!=NULL){
            count++;
            printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
            my_string = strtok(NULL, " \t\n");
        }
        sem_wait(&sems[my_rank]);
        fg_rv = fgets(my_line, 100, stdin);
        sem_post(&sems[next]);
    }
    return NULL;
}