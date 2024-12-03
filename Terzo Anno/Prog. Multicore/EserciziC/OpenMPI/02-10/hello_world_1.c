#include <stdio.h>
#include <mpi.h>
/*
Schema standard MPI
init
controllo errore
..
...

Finalize

mpicc nome -o nome -Wall
mpirun -n [NUMERO PROCESSI / CLUSTER] Nome

*/
int main(void) {
    int r = MPI_Init(NULL,NULL);   //prendo 2 puntatori, restituisce intero
    if(r != MPI_SUCCESS){           //MODO STANDARD PER CONTROLLARE SE C'è ERRORE
        printf("Errore. \n");
        MPI_Abort(MPI_COMM_WORLD,r);
    }
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD,&size); //prendo il numero di processi
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); //prendo il rank del processo

    // ORA VOGLIO CHE TUTTI I PROCESSI DIVERSI DAL RANK 0 MANDINO A 0 LA STRINGA DI HELLO

    if (rank==0){
        printf("Hello, World! Sono il processo %d di %d\n",rank,size);
        for(int i=1; i<size; i++){
            char str[256];
            MPI_Recv(str, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n",str);
        }
    }
    else{  //Processi con rank !=0
        char str[256];
        sprintf(str, "Suca, World! Sono il processo %d di %d\n",rank,size);

        MPI_Send(str, 256, MPI_CHAR, 0 , 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}


    