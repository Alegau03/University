#include <stdio.h>
int ricerca(int*, int, int,int);

int main(){
    
    int array[10];
    int i=0;

    for (i=0;i<10;i++){
        array[i]=i;
    }
    int occ=0;
    int valore= ricerca(array,10,1,occ);

    printf("Valore della funzione ricerca: %d con occorrenze: %d \n", valore,occ);

    return 0;
}

int ricerca(int* array, int dim, int x, int occorrenze){
    
    int a=0;
    for (int i=0; i<dim; i++){
        if(x==array[i]){
            a=i;
            occorrenze++;
        }
    }
    if (a==1){
        return a,occorrenze;
    }
    return 404,0;
}