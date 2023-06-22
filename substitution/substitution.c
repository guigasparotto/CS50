#include <printf.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/utils.h"

// https://cs50.harvard.edu/x/2023/psets/2/substitution/

char* encrypt(const char* key, char* text);

int main(int argc, char *argv[]) {
    const int KEY_SIZE = 26;

    char* key;
    if (argc == 2) {
        key = argv[1];
        int length = (int) strlen(key);
        if (length != KEY_SIZE) {
            printf("Key must contain 26 characters. Received: %i\n", length);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Usage: ./substitution key");
        exit(EXIT_FAILURE);
    }

    printf("plaintext:\t");
    char* text = readString();
    char* encrypted = encrypt(key, text);
    printf("ciphertext:\t%s", encrypted);
    free(encrypted);

    exit(EXIT_SUCCESS);
}

char* encrypt(const char* key, char* text) {
    char* result = allocateMemory((int) strlen(text));

    for (int i = 0, l = (int) strlen(text); i < l; i++) {
        char c = text[i];
        if (isalpha(c)) {
            int index = toupper(c) - 'A';
            char newLetter = key[index];

            // Create temporary string to hold the new letter
            char temp[2] = {0}; // Initialize with nulls

            if (isupper(c)) {
                temp[0] = (char) toupper(newLetter);
            } else {
                temp[0] = (char) tolower(newLetter);
            }

            strncat(result, temp, 1);
        } else {
            char temp[2] = {c, '\0'};
            strncat(result, temp, 1);
        }
    }

    return result;
}