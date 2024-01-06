#include "hashmap.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashmap_item.h"

typedef struct _Hashmap {
    int capacity, size;
    HashmapItem **items;
} Hashmap;

Hashmap *HashmapCreate() {
    Hashmap *map = malloc(sizeof(Hashmap));

    if (!map) {
        int error_number = errno;

        fprintf(
            stderr, 
            "Error allocating memory for Hashmap: %s\n", 
            strerror(error_number)
        );

        return NULL;
    }

    map->capacity = HASHMAP_STARTING_CAPACITY;
    map->size = 0;
    map->items = malloc(sizeof(HashmapItem*) * map->capacity);

    for (int i = 0; i < map->capacity; i++) {
        map->items[i] = NULL;
    } 

    if (!map->items) {
        int error_number = errno;

        fprintf(
            stderr, 
            "Error allocating memory for Hashmap items: %s\n", 
            strerror(error_number)
        );

        free(map);

        return NULL;
    }

    return map;
}

void HashmapToList(Hashmap *map, void **out_array, int *size) {
    *size = 0;

    for (int i = 0; i < map->capacity; i++) {
        if (map->items[i] == NULL) {
            continue;
        }

        out_array[(*size)++] = map->items[i]->data;

        HashmapItem *next_item = map->items[i]->next;

        while (next_item != NULL) {
            out_array[(*size)++] = map->items[i]->data;
        }
    }
}

void HashmapInsert(Hashmap *map, char key, void *item) {
    int index = key % map->capacity;

    map->items[index] = HashmapItemInsert(map->items[index], key, item);
}

HashmapItem *HashmapGet(Hashmap *map, char item) {
    int index = item % map->capacity;

    return HashmapItemGet(map->items[index], item);
}

bool HashmapContainsKey(Hashmap *map, char key) {
    return HashmapGet(map, key) != NULL;
}

void HashmapDestroy(Hashmap *map) {
    for (int i = 0; i < map->capacity; i++) {
        if (map->items[i] == NULL) {
            continue;
        }

        HashmapItemDestroy(map->items[i]);
    }

    free(map->items);
    free(map);
}