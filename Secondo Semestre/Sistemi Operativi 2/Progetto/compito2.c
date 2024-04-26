#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD_LEN 30

// Definizione della struttura per il nodo della parola
typedef struct WordNode {
    char word[MAX_WORD_LEN + 1];
    double probability;
    struct WordNode* next;
} WordNode;

// Definizione della struttura per la mappa delle parole
typedef struct {
    char key[MAX_WORD_LEN + 1];
    WordNode* nextWords;
} WordMap;

// Funzione per liberare la memoria allocata per i nodi delle parole
void free_word_nodes(WordNode* node) {
    while (node) {
        WordNode* next = node->next;
        free(node);
        node = next;
    }
}

// Funzione per liberare la memoria allocata per la mappa delle parole
void free_word_map(WordMap* map, int size) {
    for (int i = 0; i < size; i++) {
        free_word_nodes(map[i].nextWords);
    }
    free(map);
}

// Funzione per caricare i dati dal file CSV nella mappa delle parole
WordMap* load_csv(const char* filename, int* map_size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    WordMap* map = malloc(sizeof(WordMap) * 100); // Allocazione della memoria per la mappa delle parole
    char line[1024];
    int index = 0;

    // Lettura del file CSV e caricamento dei dati nella mappa delle parole
    while (fgets(line, sizeof(line), file) && index < 100) {
        char* token = strtok(line, ",");
        strcpy(map[index].key, token); // Copia la chiave nella mappa

        WordNode* head = NULL;
        WordNode* current = NULL;

        while ((token = strtok(NULL, ","))) {
            WordNode* new_node = malloc(sizeof(WordNode));
            // Assume che ogni parola sia seguita direttamente dalla sua probabilità
            char* comma_pos = strchr(token, ',');
            if (comma_pos) {
                *comma_pos = '\0'; // Termina la stringa alla virgola
                sscanf(comma_pos + 1, "%lf", &new_node->probability); // Leggi la probabilità
            } else {
                new_node->probability = 1; // Se non c'è una probabilità, usa 1 come default
            }
            strcpy(new_node->word, token); // Copia la parola nel nodo
            new_node->next = NULL;

            if (head == NULL) {
                head = new_node;
            } else {
                current->next = new_node;
            }
            current = new_node;
        }

        map[index++].nextWords = head; // Assegna la lista di parole al nodo della mappa
    }

    *map_size = index; // Imposta il numero effettivo di parole caricate
    fclose(file);
    printf("Caricate %d parole dal CSV.\n", *map_size); // Stampa il numero di parole caricate
    return map;
}

// Funzione per scegliere la prossima parola in base alla probabilità
char* choose_next_word(WordNode* node) {
    double rand_val = (double)rand() / RAND_MAX;
    double cumulative = 0.0;

    while (node) {
        cumulative += node->probability;
        if (rand_val <= cumulative) {
            return node->word;
        }
        node = node->next;
    }

    return NULL; // Se nessuna parola viene trovata, ritorna NULL
}

// Funzione per generare il testo basato sulla mappa delle parole
void generate_text(WordMap* map, int map_size, int num_words, const char* start_word, FILE* out) {
    char current_word[MAX_WORD_LEN + 1];
    strcpy(current_word, start_word ? start_word : ".");
    int capitalize = 1; // Flag per capitalizzare la parola successiva

    // Generazione del testo
    for (int i = 0; i < num_words; i++) {
        int found = 0; // Indica se una parola successiva è stata trovata
        for (int j = 0; j < map_size; j++) {
            if (strcmp(map[j].key, current_word) == 0) {
                char* next_word = choose_next_word(map[j].nextWords);
                if (next_word) {
                    found = 1; // Parola trovata, quindi interrompe la ricerca
                    char display_word[MAX_WORD_LEN + 1];
                    strcpy(display_word, next_word); // Copia la parola per manipolarla se necessario

                    if (capitalize && isalpha(display_word[0])) {
                        display_word[0] = toupper(display_word[0]); // Capitalizza la prima lettera della parola
                    }

                    fprintf(out, "%s ", display_word); // Stampa la parola nel file di output
                    strcpy(current_word, next_word); // Usa la parola originale per mantenere la coerenza dei dati

                    if (strchr(".?!", display_word[strlen(display_word) - 1])) {
                        capitalize = 1; // Imposta il flag se la parola finisce con un segno di punteggiatura
                    } else {
                        capitalize = 0; // Resetta il flag di capitalizzazione
                    }
                    printf("Parola corrente: %s, Parola successiva: %s\n", current_word, next_word); // Stampa di debug
                    break;
                }
            }
        }
        if (!found) {
            fprintf(out, "\nNessuna parola valida trovata per %s. Generazione interrotta.\n", current_word);
            break; // Interrompe la generazione se non viene trovata nessuna parola valida
        }
    }
}

// Funzione principale del programma
int main(int argc, char* argv[]) {
    if (argc < 4) { // Assicurati di avere abbastanza argomenti
        fprintf(stderr, "Utilizzo: %s <file CSV> <numero parole> <file output> [parola di partenza]\n", argv[0]);
        return 1;
    }

    int map_size;
    WordMap* map = load_csv(argv[1], &map_size); // Carica i dati dal file CSV nella mappa delle parole
    if (!map) return 1;
    
    srand(time(NULL)); // Inizializza il seme per la generazione di numeri casuali

    FILE* out = fopen(argv[3], "w"); // Apri il file di output
    if (!out) {
        perror("Impossibile aprire il file di output");
        return 1;
    }

    // Genera il testo basato sulla mappa delle parole
    generate_text(map, map_size, atoi(argv[2]), argc > 4 ? argv[4] : NULL, out);

    free_word_map(map, map_size); // Libera la memoria
    fclose(out); // Chiudi il file di output

    return 0;
}
