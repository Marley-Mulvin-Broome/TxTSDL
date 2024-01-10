#ifndef TUI_CONTROL_LINKED_LIST_H
#define TUI_CONTROL_LINKED_LIST_H

#include "tui_control.h"

typedef struct _FocusedControlLinkedListNode FocusedControlLinkedListNode;
typedef struct _FocusedControlLinkedList FocusedControlLinkedList;

FocusedControlLinkedList *FocusedControlLinkedListCreate();

void *FocusedControlLinkedListAdd(FocusedControlLinkedList *list, TuiControl *control);

TuiControl *FocusedControlLinkedListGetFocusedControl(FocusedControlLinkedList *list);

TuiControl *FocusedControlLinkedListPrev(FocusedControlLinkedList *list);

TuiControl *FocusedControlLinkedListNext(FocusedControlLinkedList *list);

int FocusedControlLinkedListSize(FocusedControlLinkedList *list);

void FocusedControlLinkedListDestroy(FocusedControlLinkedList *list);

#endif