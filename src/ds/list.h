#ifndef LIST_H
#define LIST_H

#define LIST_STARTING_CAPACITY 10

#include <stdbool.h>

typedef struct _List List;

List *ListCreate(void);
void ListAdd(List *list, void *data);
void ListRemove(List *list, void *data);
void ListRemoveAt(List *list, int index);
void ListRemoveAtMatch(List *list, bool (*match)(void *));
void *ListGet(List *list, int index);
int ListSize(List *list);
void ListDestroy(List *list);

#endif