#include <stdio.h>
int main() {
    int numero;
    
    printf("Inserisci un numero: ");
    scanf("%d", &numero);
    int i=0;
    printf("Hai inserito il numero: %d\n", numero);
    printf("Numeri Crescenti \n");
    while (i<=numero){
        printf("Numero: %d \n",i);
        i++;
    }
    printf("Numeri derescenti \n");
    while (i>=0){
        printf("Numero: %d \n",i);
        i--;
    }

    return 0;
}
