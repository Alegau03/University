#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
// Programma che lege input da processo con rnk 0 e li stampa negli altri processsi

int main(void){
    int rank;
    int size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (int i=0; i<size; i++){
        if (rank==i){
           if(rank==0){
            printf("Inserisci un numero per ogni processo: \n");
        for(int i=0;i<size;i++){
            
            int input;
            scanf("%d",&input);
            MPI_Send(&input,1,MPI_INT,i,0,MPI_COMM_WORLD);
        }
    }
    else{
        int input;
        MPI_Recv(&input,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("Processo %d ha ricevuto %d\n",rank,input);
    }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    



    MPI_Finalize();
    return 0;

}