#include <stdio.h>

int main(){

    printf("Inserisci il primo numero: \n");
    int n1=0;
    scanf("%d", &n1);

    printf("Primo numero: %d \n", n1);

    printf("Inserisci il secondo numero: \n");
    int n2=0;
    scanf("%d", &n2);

    printf("Primo numero: %d \n", n2);

    if (n1>n2){
        printf("Primo numero maggiore del secondo \n");
    }
    else {
        printf("Secondo numero maggiore del primo \n");
    }
    int somma = n1+n2;
    printf("Somma: %d \n",somma);
    int prodotto = n1*n2;
    printf("Prodotto: %d \n", prodotto);
    return 0;
}   