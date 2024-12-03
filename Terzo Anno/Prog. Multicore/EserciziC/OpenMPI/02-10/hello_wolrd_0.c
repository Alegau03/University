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

    printf("Hello, World! Sono il processo %d di %d\n",rank,size);

    MPI_Finalize();
    return 0;
}
