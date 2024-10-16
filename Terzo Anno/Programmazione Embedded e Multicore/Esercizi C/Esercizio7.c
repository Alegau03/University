#include <stdio.h>

int main(){

    int array1[5], array2[5];

    int i=0;

    for (i=0; i<5;i++){
        printf("Inserire il valore per array1[%d]: \n",i);
        scanf("%d",&array1[i]);
        printf("Inserire il valore per array2[%d]: \n",i);
        scanf("%d",&array2[i]);
    }
    int *p1,*p2;
    p1=&array1[0];
    p2= &array2[4];

    for(i=0; i<5;i++){
        int a= *p1;
        *p1=*p2;
        *p2=a;
        p1++;
        p2--;
    }

    printf("Stampo gli array.. \n");
    for (i=0; i<5; i++){
        printf("array1[%d]: %d \n", i, array1[i]);
        printf("array2[%d]: %d \n", i, array2[i]);
    }

    return 0;
}