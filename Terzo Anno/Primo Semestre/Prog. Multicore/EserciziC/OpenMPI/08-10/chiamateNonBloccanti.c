#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void){

    int rank,size;          //voglio mandare un intero a dx e sx e riceverne uno da dx e snx
    int send_right=19;
    int send_left=23;
    int recv_left,recv_right;
    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    //Controllo il successo di invio e ricezione da dx e snx
    MPI_Request request_send_right,request_send_left;
    MPI_Request request_recv_right,request_recv_left;
    
    // Invia a dx
    MPI_Isend(&send_right,1,MPI_INT, (rank+1)%size, 0, MPI_COMM_WORLD, &request_send_right); //l'ultimo parametro è il puntatore alla richiesta, ho bisognodi un indirizzo quindi uso &
    
    // Lo invio a (rank+1)%size perchè se rank=size-1 allora (rank+1)%size=0 se ho nodi 0 1 2 3 il nodo a dx di 3 è 0 3+1=4%4=0 

    // Invia a snx
    MPI_Isend(&send_left,1,MPI_INT, (rank-1)%size, 0, MPI_COMM_WORLD, &request_send_left); 

    // Ricevi da dx
    MPI_Irecv(&recv_right,1,MPI_INT, (rank+1)%size, 0, MPI_COMM_WORLD, &request_recv_right); 
    // Ricevi da snx
    MPI_Irecv(&recv_left,1,MPI_INT, (rank-1)%size, 0, MPI_COMM_WORLD, &request_recv_left); 


    //Dopo aver inviato e ricevuto posso andare avanti facendo altre cose basta che non uso la roba che ho inviato e ricevuto perchè le richeiste potrebbero non essere state completate

    //Esempio printo
    printf("Ciao");
    //Ora controllo con tutte le wait
    MPI_Wait(&request_send_right,MPI_STATUS_IGNORE);
    MPI_Wait(&request_send_left,MPI_STATUS_IGNORE);
    MPI_Wait(&request_recv_right,MPI_STATUS_IGNORE);
    MPI_Wait(&request_recv_left,MPI_STATUS_IGNORE);


    // Avrei potuto fare un unico WAIT usando invece che variabili di tipo MPI_Request un array di MPI_Request e controllare con WAITALL

    MPI_Finalize();
    return 0;

}