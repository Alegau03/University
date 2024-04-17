#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORD_LENGTH 31
#define MAX_SUCCESSORS 10

typedef struct {
    char word[MAX_WORD_LENGTH];
    char successors[MAX_SUCCESSORS][MAX_WORD_LENGTH];
    double frequencies[MAX_SUCCESSORS];
    int num_successors;
} WordEntry;

char* selectSuccessor(WordEntry* entry) {
    int total_frequency = 0;
    for (int i = 0; i < entry->num_successors; i++) {
        total_frequency += entry->frequencies[i];
    }

    if (total_frequency == 0) {
        // Se il totale delle frequenze è zero, restituisci casualmente una delle parole successive
        int random_index = rand() % entry->num_successors;
        return entry->successors[random_index];
    }

    double random_value = (double)rand() / RAND_MAX * total_frequency;
    double cumulative_frequency = 0;
    for (int i = 0; i < entry->num_successors; i++) {
        cumulative_frequency += entry->frequencies[i];
        if (random_value <= cumulative_frequency) {
            return entry->successors[i];
        }
    }
    return entry->successors[entry->num_successors - 1]; // Fallback in caso di errore
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file.csv> <num_words> [starting_word]\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int num_words = atoi(argv[2]);

    char *starting_word = NULL;
    if (argc >= 4) {
        starting_word = argv[3];
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Errore nell'apertura del file.\n");
        return 1;
    }

    srand(time(NULL));

    WordEntry entries[MAX_SUCCESSORS];
    int num_entries = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        strcpy(entries[num_entries].word, token);
        entries[num_entries].num_successors = 0;

        while ((token = strtok(NULL, ",")) != NULL && entries[num_entries].num_successors < MAX_SUCCESSORS) {
            strcpy(entries[num_entries].successors[entries[num_entries].num_successors], token);
            token = strtok(NULL, ",");
            entries[num_entries].frequencies[entries[num_entries].num_successors] = atof(token);
            entries[num_entries].num_successors++;
        }

        num_entries++;
    }

    fclose(file);

    printf("Testo generato:\n");

    char current_word[MAX_WORD_LENGTH];

    if (starting_word != NULL) {
        printf("Parola iniziale specificata: %s\n", starting_word);
        int starting_word_index = -1;

        for (int i = 0; i < num_entries; i++) {
            if (strcmp(entries[i].word, starting_word) == 0) {
                starting_word_index = i;
                break;
            }
        }

        if (starting_word_index != -1) {
            strcpy(current_word, starting_word);
        } else {
            printf("La parola iniziale specificata non ha successori nel file CSV. Seleziono casualmente un'altra parola iniziale.\n");
            strcpy(current_word, entries[rand() % num_entries].word);
        }
    } else {
        const char* punctuations[] = {".", "?", "!"};
        strcpy(current_word, punctuations[rand() % 3]);
    }

    current_word[0] = toupper(current_word[0]);

    printf("%s ", current_word);

    for (int i = 1; i < num_words; i++) {
        int current_entry_index = -1;

        for (int j = 0; j < num_entries; j++) {
            if (strcmp(entries[j].word, current_word) == 0) {
                current_entry_index = j;
                break;
            }
        }

        if (current_entry_index != -1) {
            strcpy(current_word, selectSuccessor(&entries[current_entry_index]));
        } else {
            printf("Errore: la parola corrente non è presente nel file CSV.\n");
            break;
        }

        if (current_word[0] != '.' && current_word[0] != '?' && current_word[0] != '!') {
            current_word[0] = tolower(current_word[0]);
        } else {
            current_word[0] = toupper(current_word[0]);
        }

        printf("%s ", current_word);
    }

    printf("\n");

    return 0;
}
