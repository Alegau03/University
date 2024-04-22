#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORD_LENGTH 30
#define MAX_LINE_LENGTH 1024

typedef struct {
    char word[MAX_WORD_LENGTH + 1];
    double probability;
} Successor;

typedef struct WordNode {
    char word[MAX_WORD_LENGTH + 1];
    Successor *successors;
    int num_successors;
    struct WordNode *next;
} WordNode;

char *trim_whitespace(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

WordNode *load_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    WordNode *head = NULL, *current = NULL;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        trim_whitespace(line);
        if (strlen(line) == 0) continue;
        
        printf("Processing line: '%s'\n", line);

        char *token = strtok(line, ",");
        if (!token) continue;

        char *word = token;
        WordNode *node = (WordNode *)malloc(sizeof(WordNode));
        if (!node) {
            fprintf(stderr, "Memory allocation failed\n");
            continue;
        }
        strcpy(node->word, word);
        node->num_successors = 0;
        node->successors = NULL;
        node->next = NULL;

        int count = 0;
        while ((token = strtok(NULL, ",")) != NULL) {
            token = trim_whitespace(token);
            if (*token == '\0') continue;
            count++;
        }

        fseek(file, -(strlen(line) + 2), SEEK_CUR);
        fgets(line, MAX_LINE_LENGTH, file);
        strtok(line, ",");

        node->num_successors = count / 2;
        node->successors = (Successor *)malloc(sizeof(Successor) * node->num_successors);
        if (!node->successors) {
            fprintf(stderr, "Memory allocation failed for successors\n");
            free(node);
            continue;
        }
        
        for (int i = 0; i < node->num_successors; i++) {
            token = strtok(NULL, ",");
            if (token == NULL) {
                fprintf(stderr, "Unexpected end of line after word '%s'\n", node->word);
                free(node->successors);
                free(node);
                continue;
            }
            token = trim_whitespace(token);
            strcpy(node->successors[i].word, token);
            token = strtok(NULL, ",");
            if (token == NULL) {
                fprintf(stderr, "Missing probability after word '%s'\n", node->successors[i].word);
                free(node->successors);
                free(node);
                continue;
            }
            token = trim_whitespace(token);
            node->successors[i].probability = atof(token);
            printf("Word: '%s', Probability: '%s'\n", node->successors[i].word, token);
        }

        if (!head) {
            head = node;
            current = node;
        } else {
            current->next = node;
            current = node;
        }
    }

    fclose(file);
    return head;
}


void generate_text(WordNode *head, int num_words, const char *start_word) {
    srand(time(NULL));  // Seed for random number generation
    FILE *file = fopen("output.txt", "w");
    if (!file) {
        perror("Error opening output file");
        return;
    }

    WordNode *current = head;
    char current_word[MAX_WORD_LENGTH + 1];

    // Find the starting word or use a random punctuation
    if (start_word == NULL) {
        const char *punctuations[] = {".", "?", "!"};
        strcpy(current_word, punctuations[rand() % 3]);
    } else {
        strcpy(current_word, start_word);
    }

    fprintf(file, "%s ", current_word);

    for (int i = 1; i < num_words; i++) {
        while (current != NULL && strcmp(current->word, current_word) != 0) {
            current = current->next;
        }
        if (current == NULL) {
            current = head; // Reset to start if no successors
            continue;
        }

        // Select a random successor based on probability
        double roll = (double)rand() / RAND_MAX;
        double cumulative = 0.0;
        int selected_index = 0;

        for (int j = 0; j < current->num_successors; j++) {
            cumulative += current->successors[j].probability;
            if (roll <= cumulative) {
                selected_index = j;
                break;
            }
        }

        strcpy(current_word, current->successors[selected_index].word);
        // Capitalize if it starts a sentence
        if (strchr(".?!", current_word[strlen(current_word) - 1]) != NULL) {
            i++;
            if (i < num_words) {
                current_word[0] = toupper(current_word[0]);
                fprintf(file, "\n%s ", current_word);
            }
        } else {
            fprintf(file, "%s ", current_word);
        }
        current = head; // Reset for next word
    }

    fclose(file);
}

void free_memory(WordNode *head) {
    while (head != NULL) {
        WordNode *temp = head;
        head = head->next;
        free(temp->successors);
        free(temp);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <csv_file> <num_words> [start_word]\n", argv[0]);
        return 1;
    }

    const char *csv_file = argv[1];
    int num_words = atoi(argv[2]);
    const char *start_word = argc > 3 ? argv[3] : NULL;

    WordNode *word_list = load_csv(csv_file);
    if (!word_list) {
        return 1;
    }

    generate_text(word_list, num_words, start_word);
    free_memory(word_list);

    printf("Text generation completed. Check 'output.txt' for the result.\n");
    return 0;
}
