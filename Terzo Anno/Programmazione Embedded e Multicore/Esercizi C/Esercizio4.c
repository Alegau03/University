#include <stdio.h>

int main(){
    int array[5];
    int i =0;
    for (i=0; i<5;i++){
        printf("Inserisci un numero");
        scanf("%d",&array[i]);
    }
    int max =array[0];
    int min=array[0];
    int somma=0;
    for(i=0; i<5;i++){
        if(array[i]<min){
            min=array[i];}
        if(array[i]>max){
            max=array[i];
        }
        somma+=array[i];
    }
    printf("Massimo: %d", max);
    printf("Minimo: %d", min);
    printf("Somma: %d", somma);
    return 0;
}