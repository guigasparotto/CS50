
#include <printf.h>
#include <stdlib.h>
#include <string.h>

void printArray(int numbers[], int size);
void bubbleSort(int numbers[], int size);
void selectionSort(int numbers[], int size);
void mergeSort(int numbers[], int start, int end);
void mergeSortRecursive(int numbers[], int size);


int main(void) {
    int numbers[] = {1, 4, 7, 9, 3, 2, 8, -9, -8, 0, 14, 11, 10};
//    int numbers[] = {8, 4, 9, 2, 5};
    int size = 13;
//    selectionSort(numbers, size);

    mergeSortRecursive(numbers, size);

    printArray(numbers, size);
}


void bubbleSort(int numbers[], int size) {
    int iterations = 0;
    for (int i = 0; i < size; i++) {
        iterations++;
        int swaps = 0;
        for (int j = 0; j < size - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                int tmp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = tmp;
                swaps++;
            }
        }

        if (swaps == 0) {
            break;
        }

        // for debugging
        // printArray(numbers, size);
    }
    printf("Iterations: %i\n", iterations);
}

void selectionSort(int numbers[], int size) {
    for (int i = 0; i < size; i++) {
        int minimum = i;
        for (int j = i + 1; j < size; j++) {
            if (numbers[j] < numbers[minimum]) {
                minimum = j;
            }
        }

        int tmp = numbers[i];
        numbers[i] = numbers[minimum];
        numbers[minimum] = tmp;

        // for debugging
//        printArray(numbers, size);
    }
}

void mergeSort(int numbers[], int start, int end) {
    if ((end - start) < 2) {
        return;
    }

    int middle = start + (end - start) / 2;
    mergeSort(numbers, start, middle);
    mergeSort(numbers, middle, end);

    // merge
    int* temp = malloc((end - start) * sizeof(int));

    int position = 0;
    int lcount = start;
    int rcount = middle;
    while (lcount < middle && rcount < end) {
        if (numbers[lcount] <= numbers[rcount]) {
            temp[position++] = numbers[lcount++];
        } else {
            temp[position++] = numbers[rcount++];
        }
    }

    while (lcount < middle) {
        temp[position++] = numbers[lcount++];
    }

    while (rcount < end) {
        temp[position++] = numbers[rcount++];
    }

    for (int i = start; i < end; i++) {
        numbers[i] = temp[i - start];
    }

    free(temp);
}

void merge(const int left[], int leftSize, const int right[], int rightSize, int result[]) {
    int i = 0, j = 0, k = 0;

    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j]) {
            result[k] = left[i];
            i++;
        } else {
            result[k] = right[j];
            j++;
        }
        k++;
    }

    // if one partition is bigger than the other after the initial loop
    while (i < leftSize) {
        result[k] = left[i];
        i++;
        k++;
    }

    while (j < rightSize) {
        result[k] = right[j];
        j++;
        k++;
    }
}

void mergeSortRecursive(int numbers[], int size) {
    if (size < 2) {
        return;
    }

    int middle = size / 2;

    // sort each half
    int* left = malloc(middle * sizeof(int));
    int* right = malloc((size - middle) * sizeof(int));
    memcpy(left, &numbers[0], middle * sizeof(int));
    memcpy(right, &numbers[middle], (size - middle) * sizeof(int));

    mergeSortRecursive(left, middle);
    mergeSortRecursive(right, size - middle);

    // merge the result
    merge(left, middle, right, size - middle, numbers);

    free(left);
    left = NULL;
    free(right);
    right = NULL;
}

void printArray(int numbers[], int size) {
    for (int k = 0; k < size; k++) {
        printf("%i, ", numbers[k]);
    }
    printf("\n");
}