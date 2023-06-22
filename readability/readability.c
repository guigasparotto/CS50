#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/utils.h"

int gradeLevel(const char* text);

int main(void) 
{
    while (true) {
        printf("Text: ");
        char* text = readString();

        if (text == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        if (strcmp(text, "quit1") == 0) {
            break;
        }

        int level = gradeLevel(text);
        if (level < 1) {
            printf("Before Grade 1\n");
        } else if (level > 16) {
            printf("Grade 16+\n");
        } else {
            printf("Grade %i\n", level);
        }

        free(text);
    }
}

bool isSentenceDelimiter(char c) {
    return c == '.' || c == '!' || c == '?';
}

double unitPer100(int dividend, int divisor) {
    return ((double) dividend / divisor) * 100;
}


// TODO: Improve this method to avoid repetition in the counters
int gradeLevel(const char* text) {
    int letters = 0;
    int words = 0;
    int sentences = 0;

    int index = 0;
    while (text[index] != '\0') {
        char c = text[index];
        if (isalpha(c)) {
            letters++;

            // This was added for cases where the sentence finishes without
            // punctuation or space... TODO: need a better solution
            if (text[index + 1] == '\0') {
                words++;
                sentences++;
            }
        } else if (ispunct(c)) {
            if (isSentenceDelimiter(c)) {
                sentences++;
                words++;
            }
        } else if (c == ' ') {
            if (!isSentenceDelimiter(text[index - 1])) {
                words++;
            }
        }
        index++;
    }

    // Coleman-Liau formula
    int gradeLevel = round((0.0588 * unitPer100(letters, words))
            - (0.296 * unitPer100(sentences, words)) - 15.8);

    return gradeLevel;
}