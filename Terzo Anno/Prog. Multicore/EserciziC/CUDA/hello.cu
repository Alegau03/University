#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ void hello(void) {
    printf("Hello World from GPU!\n");
}

int main() {
    hello<<<1, 10>>>();
    cudaDeviceSynchronize();
    return 1;
}