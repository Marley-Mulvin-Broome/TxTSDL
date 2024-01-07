#include "hashmap_item.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "charlist.h"

HashmapItem *HashmapItemCreate(uchar_t key, void *value) {
    HashmapItem *item = malloc(sizeof(HashmapItem));

    if (!item) {
        int error_number = errno;
        fprintf(stderr, "Error allocating memory for Hashmapitem: %s", strerror(error_number));
        return NULL;
    }

    item->key = key;
    item->data = value;
    item->next = NULL;

    return item;
}

HashmapItem *HashmapItemInsert(HashmapItem *item, uchar_t key, void *value) {
    if (item == NULL) {
        return HashmapItemCreate(key, value);
    }

    item->next = HashmapItemInsert(item->next, key, value);

    return item;
}

HashmapItem *HashmapItemGet(HashmapItem *item, uchar_t key) {
    if (item == NULL) {
        return NULL;
    }

    if (item->key == key) {
        return item;
    }

    return HashmapItemGet(item->next, key);
}

void HashmapItemDestroy(HashmapItem *item) {
    if (item == NULL) {
        return;
    }

    HashmapItemDestroy(item->next);

    free(item);
}