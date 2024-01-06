#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _List {
    void **data;
    int size;
    int capacity;
} List;

List *ListCreate(void) {
    List *list = malloc(sizeof(List));

    if (!list) {
        fprintf(stderr, "Failed to allocate memory for list\n");
        return NULL;
    }

    list->data = malloc(sizeof(void *) * LIST_STARTING_CAPACITY);

    if (!list->data) {
        fprintf(stderr, "Failed to allocate memory for list data\n");
        return NULL;
    }

    list->size = 0;
    list->capacity = LIST_STARTING_CAPACITY;

    return list;
}

void ListAdd(List *list, void *data) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, sizeof(void *) * list->capacity);

        if (!list->data) {
            fprintf(stderr, "Failed to reallocate memory for list data\n");
            return;
        }
    }

    list->data[list->size++] = data;
}

void ListRemove(List *list, void *data) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i] == data) {
            ListRemoveAt(list, i);
            return;
        }
    }
}

void ListRemoveAtMatch(List *list, bool (*match)(void *)) {
    for (int i = 0; i < list->size; i++) {
        if (match(list->data[i])) {
            ListRemoveAt(list, i);
            return;
        }
    }
}

void ListRemoveAt(List *list, int index) {
    if (index < 0 || index >= list->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }

    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }

    list->size--;
}

void *ListGet(List *list, int index) {
    if (index < 0 || index >= list->size) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }

    return list->data[index];

}

int ListSize(List *list) {
    return list->size;
}

void ListDestroy(List *list) {
    free(list->data);
    free(list);
}