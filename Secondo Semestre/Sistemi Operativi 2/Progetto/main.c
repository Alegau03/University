#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definizione della struttura per la coppia chiave-valore
typedef struct {
    char *key;
    int value;
} KeyValuePair;

// Definizione della struttura per il dizionario
typedef struct {
    KeyValuePair *entries;
    int size;
} Dictionary;

// Funzione per inizializzare il dizionario
Dictionary *createDictionary(int initialSize) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        fprintf(stderr, "Errore durante l'allocazione della memoria per il dizionario\n");
        exit(EXIT_FAILURE);
    }
    dict->entries = (KeyValuePair *)malloc(initialSize * sizeof(KeyValuePair));
    if (dict->entries == NULL) {
        fprintf(stderr, "Errore durante l'allocazione della memoria per le voci del dizionario\n");
        free(dict);
        exit(EXIT_FAILURE);
    }
    dict->size = initialSize;
    return dict;
}

// Funzione per aggiungere una coppia chiave-valore al dizionario
void addToDictionary(Dictionary *dict, const char *key, int value) {
    // Controlla se c'è spazio sufficiente nel dizionario
    if (dict == NULL) {
        fprintf(stderr, "Dizionario non inizializzato\n");
        exit(EXIT_FAILURE);
    }
    if (dict->size == 0) {
        fprintf(stderr, "Dizionario vuoto\n");
        exit(EXIT_FAILURE);
    }

    // Cerca la chiave nel dizionario e aggiorna il valore se esiste già
    for (int i = 0; i < dict->size; i++) {
        if (dict->entries[i].key != NULL && strcasecmp(dict->entries[i].key, key) == 0) {
            dict->entries[i].value++;
            return; // Chiave trovata e valore aggiornato
        }
    }

    // Se la chiave non è stata trovata, cerca una posizione vuota nel dizionario
    for (int i = 0; i < dict->size; i++) {
        if (dict->entries[i].key == NULL) {
            dict->entries[i].key = strdup(key);
            dict->entries[i].value = 1;
            if (dict->entries[i].key == NULL) {
                fprintf(stderr, "Errore durante l'allocazione della memoria per la chiave\n");
                exit(EXIT_FAILURE);
            }
            return; // Chiave e valore aggiunti con successo
        }
    }

    // Se non ci sono posizioni vuote, espandi il dizionario
    dict->size *= 2;
    dict->entries = realloc(dict->entries, dict->size * sizeof(KeyValuePair));
    if (dict->entries == NULL) {
        fprintf(stderr, "Errore durante l'espansione del dizionario\n");
        exit(EXIT_FAILURE);
    }

    // Aggiungi la nuova coppia chiave-valore al dizionario
    dict->entries[dict->size / 2].key = strdup(key);
    dict->entries[dict->size / 2].value = 1;
    if (dict->entries[dict->size / 2].key == NULL) {
        fprintf(stderr, "Errore durante l'allocazione della memoria per la chiave\n");
        exit(EXIT_FAILURE);
    }
}

// Funzione per liberare la memoria allocata per il dizionario
void freeDictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }

    for (int i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
    }
    free(dict->entries);
    free(dict);
}

// Funzione per stampare il contenuto del dizionario
void printDictionary(const Dictionary *dict) {
    if (dict == NULL) {
        fprintf(stderr, "Dizionario non inizializzato\n");
        return;
    }

    printf("Contenuto del dizionario:\n");
    for (int i = 0; i < dict->size; i++) {
        if (dict->entries[i].key != NULL) {
            printf("%s: %d\n", dict->entries[i].key, dict->entries[i].value);
        }
    }
}

// Funzione per convertire una stringa in minuscolo
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Utilizzo: %s <file_di_testo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];

    // Apertura del file di testo in lettura
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Impossibile aprire il file %s\n", filename);
        return EXIT_FAILURE;
    }
    
    // Creazione del dizionario
    Dictionary *dict = createDictionary(10);
    addToDictionary(dict, ".", 1);
    // Lettura del file di testo e conteggio delle occorrenze delle parole
    char word[100];
    
    while (fscanf(file, "%s", word) == 1) {

        char *ptr = word;
        
        while (*ptr != '\0') {
            if(*ptr == '.' || *ptr == '!' || *ptr == '?') {
                addToDictionary(dict, ptr, 1);
                
                 
                 }
               
            if (!(isalpha(*ptr) || *ptr == '\'' || *ptr == ',')) {
                *ptr = ' ';
            }
            ptr++;
        }
        ptr = strtok(word, " ");
        while (ptr != NULL) {
            toLowerCase(ptr); // Converti la parola in minuscolo
            addToDictionary(dict, ptr, 1);
            ptr = strtok(NULL, " ");
        }
    }

    // Chiusura del file
    fclose(file);

    // Stampa il contenuto del dizionario
    printDictionary(dict);

    // Libera la memoria allocata per il dizionario
    freeDictionary(dict);

    return EXIT_SUCCESS;
}
