#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

typedef struct node
{
    int value;
    struct node *next;
} node;


typedef struct
{
    struct node *first;
} list;


node * createNode(int value) {
    // Allocate memory and initialise its fields
    node *newNode = malloc(sizeof(node));
    newNode->value = value;
    newNode->next = NULL;

    return newNode;
}

int getSize(list* this) {
    int counter = 0;
    node *current = this->first;

    while (current != NULL) {
        current = current->next;
        counter++;
    }

    return counter;
}

void append(list* this, int value) {
    // New node containing the new value
    node *newNode = createNode(value);

    // Check what's the first NULL node and
    // replace it with the new one
    if (this->first == NULL) {
        this->first = newNode;
    } else {
        node *tmp = this->first;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }

        // Insert the new node when the current one
        // has nothing set on next
        tmp->next = newNode;
    }
}

void insert(list* this, int index, int value) {
    // New node containing the new value
    node *newNode = createNode(value);

    int listSize = getSize(this);
    if (index < 0 || listSize < index) {
        fprintf(stderr, "Index out out of range: %i, list size: %i\n", index, listSize);
        exit(EXIT_FAILURE);
    }

    // If we are inserting at the beginning
    if (index == 0) {
        newNode->next = this->first;
        this->first = newNode;
    } else {
        // save previous position and iterate through the links
        // until the correct index is located
        node *insertPos;
        node *previous = this->first;
        for (int counter = 1; counter < index; counter++) {
            previous = previous->next;
        }
        insertPos = previous->next;
        previous->next = newNode;
        newNode->next = insertPos;
    }
}

// Returns INT_MIN when the list is empty
int popIndex(list* this, int index) {
    int listSize = getSize(this);
    if (listSize == 0) {
        fprintf(stderr, "List is empty, no elements to pop.\n");
        return INT_MIN;
    } else if (index < 0 || listSize <= index) {
        fprintf(stderr, "Index out out of range: %i, list size: %i\n", index, listSize);
        exit(EXIT_FAILURE);
    }

    int value;
    if (index == 0) {
        node* oldFirst = this->first;
        value = oldFirst->value;
        this->first = oldFirst->next;
        free(oldFirst);
    } else {
        node *removePos;
        node *previous = this->first;
        for (int counter = 1; counter < index; counter++) {
            previous = previous->next;
        }

        removePos = previous->next;
        value = removePos->value;
        previous->next = removePos->next;
        free(removePos);
        removePos = NULL;
    }
    return value;
}

// Returns INT_MIN when the list is empty
int pop(list* this) {
    int listSize = getSize(this);
    if (listSize > 0) {
        return popIndex(this, listSize - 1);
    } else {
        fprintf(stderr, "List is empty, no elements to pop.\n");
        return INT_MIN;
    }
}


int main(void) {
    list lista = { NULL };
    append(&lista, 2);
    append(&lista, 3);

    printf("\n");
    insert(&lista, 1, 12);
    insert(&lista, 2, 20);

    for (node *i = lista.first; i != NULL; i = i->next) {
        printf("%i, ", i->value);
    }

    printf("\n");

    int size = getSize(&lista);
    for (int i = size - 1; i >= 0; i--) {
        printf("%i\n", popIndex(&lista, i));
    }

    printf("%i", popIndex(&lista, 0));

    for (node *i = lista.first; i != NULL; i = i->next) {
        printf("%i, ", i->value);
    }
}