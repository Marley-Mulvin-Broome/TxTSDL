#include "tui_control_linked_list.h"

#include <stdlib.h>
#include <stdio.h>

#include "tui_control.h"

typedef struct _FocusedControlLinkedListNode {
    TuiControl *control;
    struct _FocusedControlLinkedListNode *next;
    struct _FocusedControlLinkedListNode *prev;
} FocusedControlLinkedListNode;

typedef struct _FocusedControlLinkedList {
    int size;
    FocusedControlLinkedListNode *focused_control;
    FocusedControlLinkedListNode *head;
    FocusedControlLinkedListNode *tail;
} FocusedControlLinkedList;

static void destroyNode(FocusedControlLinkedListNode *node);

FocusedControlLinkedList *FocusedControlLinkedListCreate() {
    FocusedControlLinkedList *list = malloc(sizeof(FocusedControlLinkedList));

    if (!list) {
        fprintf(stderr, "Failed to allocate memory for focused control linked list\n");
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->focused_control = NULL;
    list->size = 0;

    return list;
}

void *FocusedControlLinkedListAdd(FocusedControlLinkedList *list, TuiControl *control) {
    FocusedControlLinkedListNode *node = malloc(sizeof(FocusedControlLinkedListNode));

    if (!node) {
        fprintf(stderr, "Failed to allocate memory for focused control linked list node\n");
        return NULL;
    }

    node->control = control;
    node->next = NULL;
    node->prev = NULL;

    if (!list->head) {
        list->head = node;
        list->tail = node;
        list->focused_control = node;
        node->next = node;
        node->prev = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
        node->next = list->head;
    }

    list->size++;

    return node;
}

TuiControl *FocusedControlLinkedListGetFocusedControl(FocusedControlLinkedList *list) {
    if (!list->focused_control) {
        return NULL;
    }

    return list->focused_control->control;
}

TuiControl *FocusedControlLinkedListPrev(FocusedControlLinkedList *list) {
    if (!list->focused_control) {
        return NULL;
    }

    list->focused_control = list->focused_control->prev;

    return list->focused_control->control;
}

TuiControl *FocusedControlLinkedListNext(FocusedControlLinkedList *list) {
    if (!list->focused_control) {
        return NULL;
    }

    list->focused_control = list->focused_control->next;

    return list->focused_control->control;
}

int FocusedControlLinkedListSize(FocusedControlLinkedList *list) {
    return list->size;
}

void FocusedControlLinkedListDestroy(FocusedControlLinkedList *list) {
    // If we don't do this, will seg fault as accessing memory that was already freed - cyclic linked list
    list->tail->next = NULL;

    destroyNode(list->head);
    free(list);
}

static void destroyNode(FocusedControlLinkedListNode *node) {
    FocusedControlLinkedListNode *next = node->next;

    destroyNode(next);

    free(node);
}