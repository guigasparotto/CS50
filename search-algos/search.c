
#include <printf.h>
#include <string.h>

int linearSearch(const int list[], int target, int size);
int linearStringSearch(const char *list[], char *target, int size);

int main(void) {
    int array[] = {1, 3, 4, 6, 7, 2};
    printf("%d\n", linearSearch(array, 2, 6));

    const char *words[] = {"Guilherme", "Teresa", "Alana", "Alissa"};
    printf("%d\n", linearStringSearch(words, "Alana", 4));
};

int linearSearch(const int list[], int target, int size) {
    for (int i = 0; i < size; i++) {
        if (list[i] == target) {
            return i;
        }
    }
    return  -1;
}

int linearStringSearch(const char *list[], char *target, int size) {
    for (int i =0; i < size; i++) {
        if (strcmp(list[i], target) == 0) {
            return i;
        }
    }
    return -1;
}