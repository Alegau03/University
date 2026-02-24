#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void func(void* a){
    int b;
    b= *((int*)a);
    printf("Numero: %d\n", b);
}

int main(){
    int n;
    printf("Digite um numero: ");
    scanf("%d", &n);
    pthread_t thread;
    pthread_create(&thread, NULL, func, &n);
    pthread_join(thread, NULL); // al posto di NULL potevo mettere un puntatore al valore di ritorno dalla funzione eseguita dal thread
    return 0;
}