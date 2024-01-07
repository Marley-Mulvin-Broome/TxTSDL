#include "tui_input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "../txtsdl.h"
#include "../txtsdl_screen.h"

typedef struct _TuiInput {
    int x, y;
    int width;
    int size;
    int cursor_x;
    char *text;
    TxtSDLScreen *screen;
} TuiInput;

static void inputCharacter(TuiInput *input, char value);
static void backspaceText(TuiInput *input);
static void clearScreenOfText(TuiInput *input, TxtSDLScreen *screen);

TuiInput *TuiInputCreate(int x, int y, int width, TxtSDLScreen *screen) {
    TuiInput *input = malloc(sizeof(TuiInput));

    if (!input) {
        fprintf(stderr, "Failed to allocate memory for input\n");
        return NULL;
    }

    input->text = malloc(sizeof(char) * (width + 1));

    if (!input->text) {
        fprintf(stderr, "Failed to allocate memory for input text\n");
        return NULL;
    }

    input->x = x;
    input->y = y;
    input->width = width;
    input->screen = screen;
    input->cursor_x = 0;
    input->size = 0;

    return input;
}

void TuiInputDraw(TuiInput *input) {
    TxtSDL_SetCursor(input->x + input->cursor_x, input->y);

    // TODO: draw background

    // If we don't clear the control first, removed text may still be displayed
    // clearScreenOfText(input, input->screen);

    TxtSDLScreen_WriteString(
        input->screen,
        input->x,
        input->y,
        input->text,
        &COLOUR_WHITE,
        &COLOUR_BLACK,
        false
    );
}

void TuiInputKeyPress(TuiInput *input, int key) {
    switch (key) {
        case SDLK_BACKSPACE: // Backspace
            backspaceText(input);
            break;
        default:
            inputCharacter(input, key);
            break;
    }
}

void TuiInputDestroy(TuiInput *input) {
    free(input->text);
    free(input);
}

static void inputCharacter(TuiInput *input, char value) {
    if (input->size == input->width) {
        return;
    }

    value = toupper(value);

    // Must append at cursor position
    input->text[input->cursor_x++] = value;
    input->size++;
}

static void backspaceText(TuiInput *input) {
    if (input->cursor_x <= 0) {
        return;
    }

    // Must shift all characters otherwise they disappear
    for (int i = input->cursor_x; i < input->size; i++) {
        input->text[i - 1] = input->text[i];
    }


    input->cursor_x--;
    input->size--;
    input->text[input->size] = '\0';
}

static void clearScreenOfText(TuiInput *input, TxtSDLScreen *screen) {
    TxtSDLCharCell cell = {
        .value = ' ',
        .foreground = COLOUR_WHITE,
        .background = COLOUR_BLACK
    };
    
    for (int i = 0; i < input->width; i++) {
        TxtSDLScreen_WriteChar(
            screen,
            input->x + i,
            input->y,
            &cell
        );
    }
}