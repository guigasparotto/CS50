#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

char* readString() {
    char buffer[500];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading string.\n");
        exit(EXIT_FAILURE);
    }

    // Check for the presence of newline in buffer before it's removed
    // If there is no new line, it means the text exceeded the buffer limit
    bool noNewline = strchr(buffer, '\n') == NULL;
    buffer[strcspn(buffer, "\n")] = 0; // trim newline character

    if (noNewline) {
        // Clear the remaining characters from the input stream
        // when the text exceeds the buffer character limit
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    // Allocate exactly the needed memory and copy the string there
    char* str = malloc(strlen(buffer) + 1); // +1 for null terminator
    if (str != NULL) {
        strcpy(str, buffer);
    }
    return str;
}

char* allocateMemory(int size) {
    char* result = calloc(size + 1, sizeof(char));

    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    return result;
}