#ifndef LIST_H
#define LIST_H

#define LIST_STARTING_CAPACITY 10

#include <stdbool.h>

typedef struct _List List;

typedef void (*ListIterateCallback)(void *);

/**
 * Creates a new list of void pointers.
 * @returns A pointer to the list.
*/
List *ListCreate(void);

/**
 * Adds a new element to the list.
 * @param list The list to add the element to.
 * @param data The data to add to the list.
*/
void ListAdd(List *list, void *data);

/**
 * Removes an element from the list.
 * @param list The list to remove the element from.
 * @param data The data to remove from the list.
 * @note This does not free the data.
*/
void ListRemove(List *list, void *data);

/**
 * Removes an element from the list at the specified index.
 * @param list The list to remove the element from.
 * @param index The index of the element to remove.
*/
void ListRemoveAt(List *list, int index);

/**
 * Removes an element from the list that matches the specified function.
 * @param list The list to remove the element from.
 * @param match The function to match the element with.
*/
void ListRemoveAtMatch(List *list, bool (*match)(void *));

void ListIterate(List *list, void (*callback)(void *));

/**
 * Gets an element from the list at the specified index.
*/
void *ListGet(List *list, int index);

/**
 * Gets the size of the list.
 * @param list The list to get the size of.
 * @returns The size of the list.
 * @note This is not the same as the capacity of the list.
*/
int ListSize(List *list);

/**
 * Destroys the list.
 * @param list The list to destroy.
 * @note This does not free the data in the list.
*/
void ListDestroy(List *list);

#endif