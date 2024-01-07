#ifndef TXTSDL_HASHMAP_H
#define TXTSDL_HASHMAP_H

#include "hashmap_item.h"

#include <stdbool.h>

#include "charlist.h"

#define HASHMAP_STARTING_CAPACITY 50

/**
 * uchar_t to void * hashmap
*/
typedef struct _Hashmap Hashmap;

/**
 * Creates a new char to void * hashmap
*/
Hashmap *HashmapCreate();

/**
 * Creates an array of all the items in a hashamp
 * @param map The hashmap to convert to an array
 * @param out_array The array to store the items in
 * @param size The size of the array (will be set to the size of the hashmap)
*/
void HashmapToList(Hashmap *map, void **out_array, int *size);

/**
 * Iterates over all the items in the hashmap, invoking a callback for each item
 * @param map The hashmap to iterate over
 * @param callback The callback to invoke for each item
 * @note This can be used to free all the items in the hashmap
*/
void HashmapIterate(Hashmap *map, void (*callback)(void *));

/**
 * Inserts an item into the Hashmap
 * @param map The hashmap to insert the item into
 * @param key The key to insert the item under
 * @param item The item to insert
*/
void HashmapInsert(Hashmap *map, uchar_t key, void *item);

/**
 * Gets an item from the hashmap
 * @param map The hashmap to get the item from
 * @param item The key of the item to get
*/
HashmapItem *HashmapGet(Hashmap *map, uchar_t item);

/**
 * Checks if the hashmap contains a key
 * @param map The hashmap to check
 * @param key The key to check for
*/
bool HashmapContainsKey(Hashmap *map, uchar_t key);

/**
 * Frees all the resources, besides the item data, being used by the hashmap
 * @param map The hashmap to destroy
*/
void HashmapDestroy(Hashmap *map);

#endif