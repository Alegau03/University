#include <stdio.h>
int main() {
    int numero;
    
    printf("Inserisci un numero: ");
    scanf("%d", &numero);
    int i=0;
    printf("Hai inserito il numero: %d\n", numero);
    printf("Numeri Crescenti \n");
    for(i=0; i<=numero; i++){
        printf("Numero: %d \n",i);
    }
    printf("Numeri derescenti \n");
    for(i=numero; i>=0; i--){
        printf("Numero: %d \n",i);
    }

    return 0;
}
