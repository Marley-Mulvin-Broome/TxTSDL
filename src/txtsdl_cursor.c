#include "txtsdl_cursor.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "txtsdl.h"
#include "txtsdl_colour.h"

typedef struct _TxtSDLCursor {
    int x, y;
    int width, height;
    bool visible;
} TxtSDLCursor;

TxtSDLCursor *TxtSDLCursor_Create(int x, int y, int width, int height) {
    TxtSDLCursor *cursor = malloc(sizeof(TxtSDLCursor));

    if (!cursor) {
        fprintf(stderr, "Failed to allocate memory for cursor\n");
        return NULL;
    }

    cursor->x = x;
    cursor->y = y;
    cursor->width = width;
    cursor->height = height;
    cursor->visible = false;

    return cursor;
}

void TxtSDLCursor_SetPosition(TxtSDLCursor *cursor, int x, int y) {
    cursor->x = x;
    cursor->y = y;
}

void TxtSDLCursor_SetVisible(TxtSDLCursor *cursor, bool visible) {
    cursor->visible = visible;
}

void TxtSDLCursor_Draw(TxtSDLCursor *cursor, int cell_width, int cell_height) {
    if (!cursor->visible) {
        return;
    }

    SDL_Rect rect = {
        cursor->x * cell_width,
        cursor->y * cell_height + cell_height,
        cursor->width,
        cursor->height
    };

    TxtSDL_SetDrawColour(&COLOUR_WHITE);

    TxtSDL_DrawRect(
        &rect,
        true
    );
}

void TxtSDLCursor_Destroy(TxtSDLCursor *cursor) {
    free(cursor);
}