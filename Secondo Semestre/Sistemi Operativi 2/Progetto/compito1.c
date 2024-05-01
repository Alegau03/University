    #include <stdio.h>
    #include <wchar.h>
    #include <stdlib.h>
    #include <locale.h>

    #define MAX_WORD_SIZE 100

    // Struttura per memorizzare la chiave e la parola successiva
    typedef struct {
        wchar_t key[MAX_WORD_SIZE];
        wchar_t next_word[MAX_WORD_SIZE];
    } KeyNextWordPair;

    int main(int argc, char *argv[]) {
        // Impostazione della localizzazione per supportare i caratteri wide
        setlocale(LC_ALL, "");

        // Apertura del file di testo
        char *filename= argv[1];
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            fprintf(stderr, "Impossibile aprire il file.\n");
            return 1;
        }

        // Inizializzazione della struttura per memorizzare la coppia chiave-parola
        KeyNextWordPair pair;
        wchar_t key[MAX_WORD_SIZE];
        wscanf(L"%ls", key); // Input della chiave da cercare

        int found = 0;

        // Lettura del file e ricerca della chiave
        while (fscanf(file, "%ls", pair.key) != EOF) {
            // Se la chiave corrisponde, leggi la parola successiva
            if (wcscmp(pair.key, key) == 0) {
                if (fscanf(file, "%ls", pair.next_word) != EOF) {
                    found = 1;
                    break;
                }
            }
        }

        // Chiusura del file
        fclose(file);

        // Se la chiave è stata trovata, stampa la coppia chiave-parola successiva
        if (found) {
            wprintf(L"Chiave: %ls\n", pair.key);
            wprintf(L"Parola successiva: %ls\n", pair.next_word);
        } else {
            wprintf(L"Chiave non trovata.\n");
        }

        return 0;
    }
