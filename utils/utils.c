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

bool compareString(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return false;
    }

    int size1 = 0;
    while (s1[size1] != '\0') {
        size1++;
    }

    int size2 = 0;
    while (s2[size2] != '\0') {
        size2++;
    }

    if (size1 != size2) {
        return false;
    }

    for (int i = 0; i < size1; i++) {
        if (*(s1 + i) != *(s2 + i)) {
            return false;
        }
    }
    return true;
}

char* copyString(const char *s1) {
    if (s1 == NULL) {
        return NULL;
    }

    int length = (int) strlen(s1);
    char *copy = malloc(length + 1);

    // checks if malloc failed to allocate memory, this is a serious
    // issue and might indicate that the computer is out of memory
    if (copy == NULL) {
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        // *(copy + i) = *(s1 + i)
        copy[i] = s1[i];
    }

    // add the null terminator after copying all chars
    copy[length] = '\0';

    return copy;
}