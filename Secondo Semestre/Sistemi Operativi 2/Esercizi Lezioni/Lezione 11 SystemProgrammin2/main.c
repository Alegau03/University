#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Questo programma chiede in input il nome di un file (DEVE ESISTERE) e un file di output
// Alloca dinamicamente un blocco di memoria della grandezza del file in input
// Apre il file di inpout e ne salva il contenuto nel blocco allocato dinamicamente
// Infine scrive nel file di output ciò che c'è nel blocco allocato

int main(int argc, char *argv[]) {
    char filein[100];
    char fileout[100];

    // Input dei nomi dei file
    printf("Inserisci il nome del file di input: ");
    fgets(filein, sizeof(filein), stdin);
    filein[strcspn(filein, "\n")] = 0;

    printf("Inserisci il nome del file di output: ");
    fgets(fileout, sizeof(fileout), stdin);
    fileout[strcspn(fileout, "\n")] = 0;

    // Apertura del file di input
    int fdin = open(filein, O_RDONLY);
    if (fdin == -1) {
        fprintf(stderr, "Impossibile aprire il file di input\n");
        return 1;
    }

    // Determinazione della dimensione del file di input
    struct stat st;
    if (fstat(fdin, &st) == -1) {
        fprintf(stderr, "Impossibile ottenere le informazioni sul file di input\n");
        close(fdin);
        return 1;
    }
    off_t size = st.st_size;

    // Allocazione del blocco di memoria per contenere i dati del file
    char *m1 = malloc(size);
    if (m1 == NULL) {
        fprintf(stderr, "Memoria insufficiente\n");
        close(fdin);
        return 1;
    }

    // Lettura dal file di input nel blocco di memoria m1
    ssize_t bytes_read = read(fdin, m1, size);
    if (bytes_read != size) {
        fprintf(stderr, "Errore durante la lettura dal file di input\n");
        close(fdin);
        free(m1);
        return 1;
    }

    // Chiusura del file di input
    close(fdin);

    // Apertura del file di output
    int fdout = open(fileout, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fdout == -1) {
        fprintf(stderr, "Impossibile aprire/creare il file di output\n");
        free(m1);
        return 1;
    }

    // Scrittura del blocco di memoria m1 nel file di output
    ssize_t bytes_written = write(fdout, m1, size);
    if (bytes_written != size) {
        fprintf(stderr, "Errore durante la scrittura nel file di output\n");
        free(m1);
        close(fdout);
        return 1;
    }

    // Chiusura del file di output
    close(fdout);

    // Libera la memoria allocata per il blocco m1
    free(m1);

    return 0;
}
