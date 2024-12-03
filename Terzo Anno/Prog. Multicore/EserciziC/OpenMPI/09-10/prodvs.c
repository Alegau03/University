// ESERCIZIO 3.9
/*

Write an MPI program that implements multiplication of a vector by a scalar
and dot product. The user should enter two vectors and a scalar, all of which are
read in by process 0 and distributed among the processes. The results are
calculated and collected onto process 0, which prints them. You can assume that n,
the order of the vectors, is evenly divisible by comm sz. */


#include <stdio.h>
#include <mpi.h>



int main(void){
    int size,rank;
    int local_dot_product = 0; // Prodotto scalare locale
    int total_dot_product = 0;  // Prodotto scalare totale

    int scalare;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int v1[size];
    int v2[size];
    int v1_scalare[size];
    int v2_scalare[size];
    for(int i=0; i<size; i++){
        v1_scalare[i]=0;
        v2_scalare[i]=0;
    }
    if (rank==0){
        printf("La lunghezza dei vettori è %d:  \n", size);
       

        printf("Inserire i numeri per il primo vettore: \n");

        for (int i=0; i<size; i++){
            scanf("%d",&v1[i]);
        }

        printf("Inserire i numeri per il secondo vettore: \n");

        for (int i=0; i<size; i++){
            scanf("%d",&v2[i]);
        }

        printf("Inserire uno scalare: \n");
        scanf("%d", &scalare);

    }

    
    int local_v1, local_v2; 
    


    // Broadcast dei vettori a tutti i processi
    MPI_Bcast(&scalare, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(v1, size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(v2, size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcolo locale
    local_v1 = v1[rank];
    local_v2 = v2[rank];
    
    v1_scalare[rank] = local_v1 * scalare; // Vettore scalato 1
    v2_scalare[rank] = local_v2 * scalare; // Vettore scalato 2
    local_dot_product = local_v1 * local_v2; // Prodotto scalare locale
        
    
    MPI_Reduce(&local_dot_product,&total_dot_product,1,MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Invio dei risultati scalati a processo 0
    if (rank != 0) {
        MPI_Send(v1_scalare + rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(v2_scalare + rank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } else {
        // Processo 0 riceve i risultati scalati dai processi rimanenti
        for (int i = 1; i < size; i++) {
            MPI_Recv(v1_scalare + i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(v2_scalare + i, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }


    if (rank == 0) {
        printf("DotProduct è uguale a : %d \n", total_dot_product);
        printf("Il vettore 1 dopo il prodotto per lo scalare è: \n");
        for (int i = 0; i < size; i++) {
            printf("%d \n", v1_scalare[i]);
        }
        printf("Il vettore 2 dopo il prodotto per lo scalare è: \n");
        for (int i = 0; i < size; i++) {
            printf("%d \n", v2_scalare[i]);
        }
    }

    MPI_Finalize();
    return 0;
}