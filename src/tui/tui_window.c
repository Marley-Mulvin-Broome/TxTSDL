#include "tui_window.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tui_control.h"
#include "tui_control_linked_list.h"
#include "../txtsdl.h"
#include "../txtsdl_screen.h"
#include "../txtsdl_colour.h"
#include "../txtsdl_events.h"
#include "../ds/list.h"
#include "../ds/charlist.h"



typedef struct _TuiWindow {
    String *title;
    int x, y;
    int width, height;
    FocusedControlLinkedList *focusable_controls;
    List *children;
    TxtSDLScreen *screen;
} TuiWindow;

static void keypressHandler(TxTSDLKeyEvent *event, void *data);

TuiWindow *TuiWindowCreate(
    const char *title, int x, int y, int width, int height, TxtSDLScreen *screen
) {
    TuiWindow *window = malloc(sizeof(TuiWindow));

    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
    window->screen = screen;

    if (!window) {
        fprintf(stderr, "Failed to allocate memory for window\n");
        return NULL;
    }

    window->title = StringFromCString(title);

    if (!window->title) {
        fprintf(stderr, "Failed to allocate memory for window title\n");
        return NULL;
    }
    
    window->children = ListCreate();

    if (!window->children) {
        fprintf(stderr, "Failed to create list for window children\n");
        return NULL;
    }

    window->focusable_controls = FocusedControlLinkedListCreate();

    if (!window->focusable_controls) {
        fprintf(stderr, "Failed to create focused control linked list for window\n");
        return NULL;
    }

    TxtSDL_AddKeyPressEventHandler(
        keypressHandler,
        window
    );

    return window;
}

void TuiWindowDraw(TuiWindow *window) {
    TxtSDLScreen_WriteString(
        window->screen,
        window->x,
        window->y,
        window->title,
        &COLOUR_WHITE,
        &COLOUR_BLACK,
        false
    );

    for (int i = 0; i < ListSize(window->children); i++) {
        TuiControl *child = ListGet(window->children, i);
        TuiControlDraw(child, window->screen);
    }
}

void TuiWindowAddChild(TuiWindow *window, TuiControl *child) {
    ListAdd(window->children, child);

    if (TuiControlFocusable(child)) {
        FocusedControlLinkedListAdd(window->focusable_controls, child);
        
        if (FocusedControlLinkedListSize(window->focusable_controls) == 1) {
            TuiControlFocus(child);
        }
    }
}

void TuiWindowKeyPress(TuiWindow *window, TxTSDLKeyEvent *event) {
    // Control key presses -> tab
    if (event->key == SDLK_TAB) {
        TuiWindowNextFocus(window);
        return;
    }

    for (int i = 0; i < ListSize(window->children); i++) {
        TuiControl *child = ListGet(window->children, i);
        TuiControlKeyPress(child, event);
    }
}

void TuiWindowNextFocus(TuiWindow *window) {
    TuiControlUnfocus(
        FocusedControlLinkedListGetFocusedControl(window->focusable_controls)
    );

    TuiControlFocus(
        FocusedControlLinkedListNext(window->focusable_controls)
    );
}

void TuiWindowDestroy(TuiWindow *window) {
    StringDestroy(window->title);

    for (int i = 0; i < ListSize(window->children); i++) {
        TuiControl *child = ListGet(window->children, i);
        TuiControlDestroy(child);
    }

    ListDestroy(window->children);
    free(window);
}

static void keypressHandler(TxTSDLKeyEvent *event, void *data) {
    TuiWindowKeyPress(data, event);
}