#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int* create_random_vector(int n){
    srand(time(NULL));
    int* v = (int*)malloc(n*sizeof(int));
    for (int i=0; i<n; i++){
        v[i] = rand() % 10;
    }
    return v;
}
void print_vec(int *v, int n){
    printf("[ ");
    for (int i=0; i<n; i++){
        printf("%d ", v[i]);
    }
    printf("]");
    printf("\n");
}
int main(int argc, char** argv){
    int rank,size;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int *a,*b;
    int n= atoi(argv[1]);

    if (n%size!=0){
        printf("The number of elements must be divisible by the number of processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }


    if (rank==0){
        a=create_random_vector(n);
        b= create_random_vector(n);
        printf("Vettore a\n");
        print_vec(a,n);
        printf("Vettore b\n");
        print_vec(b,n);
        MPI_Scatter(a,n/size,MPI_INT,MPI_IN_PLACE,n/size,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Scatter(b,n/size,MPI_INT,MPI_IN_PLACE,n/size,MPI_INT,0,MPI_COMM_WORLD);

    }else{
        a = (int*)malloc(n/size*sizeof(int));
        b = (int*)malloc(n/size*sizeof(int));
        MPI_Scatter(NULL,n/size,MPI_INT,a,n/size,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Scatter(NULL,n/size,MPI_INT,b,n/size,MPI_INT,0,MPI_COMM_WORLD);

    }
    printf("Sono rank %d e ho ricevuto il vettore\n",rank);
    print_vec(a,n/size);
    print_vec(b,n/size);
    int* c= (int*)malloc(n/size*sizeof(int));
    for (int i=0; i<n/size; i++){
        c[i] = a[i] + b[i];
    }
    MPI_Barrier(MPI_COMM_WORLD);

    int* c_finale=NULL;
    if (rank==0){
        c_finale = (int*)malloc(n*sizeof(int));
    }
    MPI_Gather(c,n/size,MPI_INT,c_finale,n/size,MPI_INT,0,MPI_COMM_WORLD);
    
    if (rank==0){
        printf("Vettore somma\n");
        print_vec(c_finale,n);

    }   
    free(a);
    free(b);
    free(c);
   


    MPI_Finalize();
    return 0;
}