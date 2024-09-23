#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    int size = 1;

    if (argc>1) {
        size = atoi(argv[1]);
    }

    int *m1 = malloc(size);
    int *m2 = malloc(size);

    if (m1 == NULL || m2 == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int c = atoi(argv[2]);
    m1= memset(m1,c,size*sizeof(int));
    memcpy(m2, m1, size*sizeof(int));

    for(int i=0; i<size; i++) {
        printf("%d\n", m2[i]);
    }
    
    free(m1);
    free(m2);
    return 0;
}