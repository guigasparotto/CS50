// Implements a dictionary's functionality

#include <stdbool.h>
#include <printf.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// The functions below are part of the exercises available in the Harvard CS50 course.
// The exercise requires the implementation of a spell checker that loads a dictionary and
// a text file, then check the text for misspelled words.
// To run the program and try it out, follow the link below and extract the zip file
// to a new project, then replace the dictionary.c file by this one
// https://cs50.harvard.edu/x/2023/psets/5/speller/

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets - supports 26 * 26 * 26
// Considering the permutations possible when
// combining the 3 letters
const unsigned int N = 17576;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
// according to the spec, all words in the dict are lowercase
bool check(const char *word)
{
    char word_copy[LENGTH + 1];
    unsigned long len = strlen(word);
    for (int i = 0; i < len; i++) {
        word_copy[i] = tolower(word[i]);
    }
    word_copy[len] = '\0';

    unsigned int hash_value = hash(word);
    unsigned int pos = hash_value % N;
    node *current = table[pos];

    while (current != NULL) {
        if (strcmp(current->word, word_copy) == 0) {
            return true;
        }
        current = current->next;
    }

    return false;
}

unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;

    // Use first 4 chars to build the hash
    // 4 presented the best performance during tests
    for(int i = 0; i < 4; ++i) {
        if (word[i] == '\0') {
            break;
        }

        hash_value = 31 * hash_value + tolower(word[i]);
    }

    return hash_value;
}

// Function either fails or return true, need to investigate
// what cases should return false
bool insert(char *word, unsigned int hash) {
    unsigned int pos = hash % N;
    node *newItem = malloc(sizeof(node));
    if (newItem == NULL) {
        fprintf(stderr, "Failed to allocate memory for newItem insert.\n");
        return EXIT_FAILURE;
    }

    strcpy(newItem->word, word);
    newItem->next = NULL;

    if (table[pos] == NULL) {
        table[pos] = newItem;
    } else {
        newItem->next = table[pos];
        table[pos] = newItem;
    }

    return true;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    char line[LENGTH + 1];  // buffer to hold each line
    while (fgets(line, sizeof(line), file) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (line[0] == '\0') {
            continue;
        }

        unsigned int hash_value = hash(line);
        insert(line, hash_value);
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
// and the function assumes the file finishes with a \n (new line) char,
// if this is not the case, a clause should be added to increment size in 1
unsigned int size(void)
{
    unsigned int size = 0;

    for (int i = 0; i < N; i++) {
        node *current = table[i];
        while (current != NULL) {
            size++;
            current = current->next;
        }
    }

    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++) {
        node *current = table[i];
        while (current != NULL) {
            node *tmp = current;
            current = current->next;
            free(tmp);
        }
        table[i] = NULL;
    }
    return true;
}
