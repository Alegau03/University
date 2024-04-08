#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Questo programma prende due file apre il primo, copia il contenuto in un area di memoria
// Apre il secondo e lo alloca in un area di memoria
// Copia la prima area nella seconda 
// Casta i Caratteri in minuscolo in MAIUSCOLO e viceversa

int main(int argc, char *argv[]) {
    void *scr;
    void *scrout;
    char *fileName = argv[1];
    char *fileOutput = argv[2];
    int fd = open(fileName, O_RDONLY);
    
    struct stat statbuf;
    fstat(fd,&statbuf);
    scr = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    
    int fout= open(fileOutput, O_RDWR | O_CREAT, 0666);
    ftruncate(fout, statbuf.st_size);
    scrout = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fout, 0);
    memcpy(scrout, scr, statbuf.st_size);

    for (int i = 0; i < statbuf.st_size; i++) {
       if (97 <= ((char *)scr)[i] && ((char *)scr)[i] <= 122) {
           ((char *)scrout)[i] = ((char *)scr)[i] - 32;
       }
       else if (65 <= ((char *)scr)[i] && ((char *)scr)[i] <= 90) {
           ((char *)scrout)[i] = ((char *)scr)[i] + 32;
       }
       else{
           ((char *)scrout)[i] = ((char *)scr)[i];
       }
    }


    int c = munmap(scr,statbuf.st_size);
    int c2 = munmap(scrout,statbuf.st_size);
    close(fout);
    close(fd);
}