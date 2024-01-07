#ifndef HASHMAP_ITEM_H
#define HASHMAP_ITEM_H

#include "charlist.h"

/**
 * Represents an item internally stored in a hashmap.
 * Acts as a linked list for items with the same hash.
*/
typedef struct _HashmapItem {
    void *next;
    void *data;
    uchar_t key;
} HashmapItem;

/**
 * Creates a new HashmapItem
 * @param key The key to store the item under
 * @param value The value to store
*/
HashmapItem *HashmapItemCreate(uchar_t key, void *value);

/**
 * Inserts an item into the HashmapItem
 * @param item The item to insert into
 * @param key The key to store the item under
*/
HashmapItem *HashmapItemInsert(HashmapItem *item, uchar_t key, void *value);

/**
 * Gets an item from the HashmapItem
 * @param item The item to get the item from
 * @param key The key of the item to get
*/
HashmapItem *HashmapItemGet(HashmapItem *item, uchar_t key);

/**
 * Frees all the resources, besides the item data, being used by the HashmapItem
 * @param item The HashmapItem to destroy
*/
void HashmapItemDestroy(HashmapItem *item);

#endif