#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int thread_count;
int n;
double sum;
void* Thread_sum(void* rank){
    long my_rank= (long) rank;
    double factor=1.0;
    long int i;
    long long my_n= n/thread_count;
    long long my_first_i= my_n*my_rank;
    long long my_last_i= my_first_i+my_n;


    if (my_first_i%2==0){
        factor=1.0;
    }else{
        factor= -(1.0);
    }

    for(i=my_first_i; i<my_last_i;i++, factor =-factor){
        sum+=factor/(2*i+1);
    }
    printf("Somma nel thread %ld: %f \n", my_rank, sum);

    return NULL;
}

int main(int argc, char* argv[]){
    double pi;
    long thread;
    pthread_t* thread_handles; //array di tutti i thread
    sum=0;
    thread_count= strtol(argv[1], NULL, 10);
    n= strtol(argv[2],NULL,10);
    thread_handles= malloc(thread_count*sizeof(pthread_t));
    
    for (thread=0; thread<thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*) thread);
    }
    
     
    for (thread=0; thread<thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
        
    }
    printf("Somma: %f\n",sum);
    pi=4*sum;
    printf("Pi estimated: %f\n", pi);
    return 0;
}