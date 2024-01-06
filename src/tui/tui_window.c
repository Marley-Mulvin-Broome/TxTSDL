#include "tui_window.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tui_control.h"
#include "../txtsdl.h"
#include "../txtsdl_screen.h"
#include "../txtsdl_colour.h"
#include "../txtsdl_events.h"
#include "../ds/list.h"


typedef struct _TuiWindow {
    char *title;
    int x;
    int y;
    int width;
    int height;
    List *children;
    TxtSDLScreen *screen;
} TuiWindow;

static void keypressHandler(TxTSDLKeyEvent *event, void *data);

TuiWindow *TuiWindowCreate(
    const char *title, int x, int y, int width, int height, TxtSDLScreen *screen
) {
    TuiWindow *window = malloc(sizeof(TuiWindow));

    if (!window) {
        fprintf(stderr, "Failed to allocate memory for window\n");
        return NULL;
    }

    window->title = malloc(sizeof(char) * (strlen(title) + 1));

    if (!window->title) {
        fprintf(stderr, "Failed to allocate memory for window title\n");
        return NULL;
    }

    strcpy(window->title, title);
    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
    window->screen = screen;
    window->children = ListCreate();

    if (!window->children) {
        fprintf(stderr, "Failed to create list for window children\n");
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
}

void TuiWindowKeyPress(TuiWindow *window, TxTSDLKeyEvent *event) {
    for (int i = 0; i < ListSize(window->children); i++) {
        TuiControl *child = ListGet(window->children, i);
        TuiControlKeyPress(child, event);
    }
}

void TuiWindowDestroy(TuiWindow *window) {
    free(window->title);
    ListDestroy(window->children);
    free(window);
}

static void keypressHandler(TxTSDLKeyEvent *event, void *data) {
    TuiWindowKeyPress(data, event);
}