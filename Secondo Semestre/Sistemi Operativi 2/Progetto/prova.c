#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> 


#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 1000


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

//FUNZIONI DI MODIFICA FILE

static void skip_copied_bytes(char *str, int *i)
{
    char c = 0;
    while (*str != '\0' && *str == ' ')
    {
        str++;
        (*i)++;
    }
    
    while (*str != '\0' && *str != ' ' && !ispunct(c))
    {
        str++;
        (*i)++;
        c = *str;
    }

    while (*str != '\0' && *str == ' ')
    {
        str++;
        (*i)++;
    }
}

static int count_char(char *str)
{

    int count = 0;
    char c = 0;
    while (*str != '\0' && *str == ' ')
        str++;


    while (*str != '\0' && *str != ' ' && !ispunct(c))
    {
        
        if (*str == ',') {
            *str = ' ';
        }

        count++;
        str++;
        c = *str;
    }

    while (*str != '\0' && *str == ' ')
    {
        str++;
    }

    return count;
}

static int count_word(char const *s, char c)
{
    int i;
    int count;

    count = 0;
    i = 0;
    while (s[i] != '\0')
    {
        while (s[i] == c)
            i++;
        if (s[i] != '\0')
            count++;
        while (s[i] != c && s[i] != '\0') {
            if (ispunct(s[i]))
                count++;
            i++;
        }
    }

    return count;
}
void rimuoviVirgole(const char *filename) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        fprintf(stderr, "Impossibile aprire il file %s\n", filename);
        return;
    }

    int ch;
    long int writePos = 0; // Posizione di scrittura nel file
    long int readPos = 0;  // Posizione di lettura nel file

    while ((ch = fgetc(file)) != EOF) {
        if (ch != ',' && ch !=':' && ch !=';') {
            fseek(file, writePos++, SEEK_SET); // Imposta la posizione di scrittura
            fputc(ch, file); // Scrive il carattere nel file
        }
        readPos++; // Aggiorna la posizione di lettura
    }

    // Tronca il file alla nuova dimensione (rimuove eventuali caratteri residui)
    ftruncate(fileno(file), writePos);

    fclose(file);

    //printf("Le virgole sono state rimosse da %s\n", filename);
}
void modificaFile(const char *nomeFile) {
    rimuoviVirgole(nomeFile);
    FILE *file = fopen(nomeFile, "r+");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return;
    }

    char str[1000];
    if (fscanf(file, "%999[^\n]", str) != 1) { // Verifica la corretta lettura dalla stringa
        fclose(file);
        printf("Errore nella lettura dal file");
        return;
    }

    char **array;
    int i = 0;
    int j = 0;
    int size = 0;

    size = count_word(str, ' ');
    if((array = malloc(sizeof(char *) * (size + 1))) == NULL) {
        fclose(file);
        return;
    }

    while (str[i])
    {
        size = count_char(&str[i]);
        if ((array[j] = malloc(sizeof(char) * (size + 1))) == NULL) { // Correzione dell'allocazione della memoria
            fclose(file);
            return;
        }

        strncpy(array[j], &str[i], size);
        array[j][size] = '\0';

        skip_copied_bytes(&str[i], &i);
        j++;
    }

    array[j] = NULL;

    // Scrive la nuova stringa nel file
    rewind(file);
    for (i = 0; array[i]; i++) {
        fprintf(file, "%s ", array[i]);
    }
    fclose(file);

    // Libera la memoria allocata
    for(i = 0; array[i]; i++) {
        free(array[i]);
    }
    free(array);

    //printf("Il file è stato modificato con successo!\n");
}

// FINE FUNZIONE MODIFICA FILE


// Funzione per aggiungere una coppia chiave-valore al dizionario
void addFreqToDictionary(Dictionary *dict, const char *key, int value) {
    //printf("DEBUG: Inizio addFreqToDictionary\n");
    
    // Controlla se il dizionario è stato inizializzato correttamente
    if (dict == NULL) {
        fprintf(stderr, "Dizionario non inizializzato\n");
        exit(EXIT_FAILURE);
    }

    // Check if the key is not null
    if (key == NULL) {
        fprintf(stderr, "Chiave non valida\n");
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: Dimensione del dizionario: %d\n", dict->size);

    // Cerca la chiave nel dizionario e aggiorna il valore se esiste già
    for (int i = 0; i < dict->size; i++) {
        //printf("DEBUG: Iterazione %d\n", i);
        //printf("DEBUG: Chiave: %s\n", dict->entries[i].key);
        if (dict->entries[i].key != NULL && strcasecmp(dict->entries[i].key, key) == 0) {
            dict->entries[i].value++;
            //printf("DEBUG: Valore aggiornato per la chiave %s\n", key);
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
            //printf("DEBUG: Chiave e valore aggiunti con successo\n");
            return; // Chiave e valore aggiunti con successo
        }
    }

    // Se non ci sono posizioni vuote, espandi il dizionario
    //printf("DEBUG: Espansione del dizionario\n");
    dict->size *= 2;
    dict->entries = realloc(dict->entries, dict->size * sizeof(KeyValuePair));
    //printf("DEBUG: Nuova dimensione del dizionario dopo l'espansione: %d\n", dict->size);

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
    //printf("DEBUG: Nuova coppia chiave-valore aggiunta al dizionario\n");
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
Dictionary aggiornaFrequenza(FILE *file, Dictionary *dict) {
    
    
    addFreqToDictionary(dict, ".", 1);
    // Lettura del file di testo e conteggio delle occorrenze delle parole
    
    char word[100];
    
    while (fscanf(file, "%s", word) == 1) {

        char *ptr = word;
        
        while (*ptr != '\0') {
            if(*ptr == '.' || *ptr == '!' || *ptr == '?') {
                addFreqToDictionary(dict, ptr, 1);
                
                 
                 }
            
            if (!(isalpha(*ptr) || *ptr == '\'' || *ptr == ',')) {
                *ptr = ' ';
            }
            ptr++;
        }
        ptr = strtok(word, " ");
        while (ptr != NULL) {
            toLowerCase(ptr); // Converti la parola in minuscolo
            addFreqToDictionary(dict, ptr, 1);
            ptr = strtok(NULL, " ");
        }
    }

    


    return *dict;
}

char* searchWord(const char* filename, const char* targetWord) {
    //printf("Cerco la parola %s nel file %s\n", targetWord, filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Impossibile aprire il file %s.\n", filename);
        exit(1);
    }
    if (strcmp(targetWord, "?") == 0 || strcmp(targetWord, "!") == 0 || strcmp(targetWord, ".") == 0) {
    
    }
    char line[MAX_LINE_LENGTH];
    char *result = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria.\n");
        exit(1);
    }

    strcpy(result, "");

    while (fgets(line, sizeof(line), file)) {
        
        char *token = strtok(line, " ");
       
        while (token != NULL) {
            
            if (strcmp(token, targetWord) == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    strcat(result, token);
                    strcat(result, ",1,");
                }
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(file); 
    return result;
}


void scorriDizionario(const char *filename,Dictionary *dict) {
    
    char stringa[100];
    int fine= dict->size;
    //printf("Fine: %d\n",fine);
    for (int i = 0; i < fine; i++) {
        if( i== fine-2) {
                
                break;
            }
        if (dict->entries[i].key != NULL) {
            if (i==0) {
                printf("%s: %s,1\n", dict->entries[i].key,dict->entries[i+1].key);
                continue;
            }
            
            char* stringa= searchWord(filename,dict->entries[i].key);
            printf("%s: %s\n", dict->entries[i].key,stringa);
            if (strcmp(stringa, "") == 0) {
                continue;
            }
            
        }
    }
    
}
void compitoUno(const char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        fprintf(stderr, "Impossibile aprire il file %s\n", filename);
        return;
    }
   

    Dictionary *dizionarioFrequenze = createDictionary(10);
   
    *dizionarioFrequenze = aggiornaFrequenza(file,dizionarioFrequenze);
   
   
    fclose(file);
    
    //modificaFile(filename); //NON PROVOCA CORE DUMP ma funziona solo alla seconda chiamata
    
    //printDictionary(dizionarioFrequenze);
    scorriDizionario(filename,dizionarioFrequenze);
    
    freeDictionary(dizionarioFrequenze);
    
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Utilizzo: %s <file_di_testo> <numeroCompito>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    int compito = atoi(argv[2]);
    if (compito==1) {
        //modificaFile(filename);
        printf("Compito 1\n");

        compitoUno(filename);
    }
    else {
        printf("Compito 2\n");
    }
    //modificaFile(filename); //Provoca coredump
    
    // Apertura del file di testo in lettura
    
    return EXIT_SUCCESS;
    
}
