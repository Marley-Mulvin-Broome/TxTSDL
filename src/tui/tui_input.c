#include "tui_input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "../txtsdl.h"
#include "../txtsdl_screen.h"
#include "../ds/charlist.h"

typedef struct _TuiInput {
    int x, y;
    int width;
    int cursor_x;
    String *text;
    TxtSDLScreen *screen;
} TuiInput;

static void inputCharacter(TuiInput *input, char value);
static void backspaceText(TuiInput *input);

TuiInput *TuiInputCreate(int x, int y, int width, TxtSDLScreen *screen) {
    TuiInput *input = malloc(sizeof(TuiInput));

    if (!input) {
        fprintf(stderr, "Failed to allocate memory for input\n");
        return NULL;
    }

    input->text = StringCreate();

    if (!input->text) {
        fprintf(stderr, "Failed to allocate memory for input text\n");
        return NULL;
    }

    input->x = x;
    input->y = y;
    input->width = width;
    input->screen = screen;
    input->cursor_x = 0;

    return input;
}

void TuiInputDraw(TuiInput *input) {
    TxtSDL_SetCursor(input->x + input->cursor_x, input->y);

    // TODO: draw background

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
        case SDLK_a:
        case SDLK_b:
        case SDLK_c:
        case SDLK_d:
        case SDLK_e:
        case SDLK_f:
        case SDLK_g:
        case SDLK_h:
        case SDLK_i:
        case SDLK_j:
        case SDLK_k:
        case SDLK_l:
        case SDLK_m:
        case SDLK_n:
        case SDLK_o:
        case SDLK_p:
        case SDLK_q:
        case SDLK_r:
        case SDLK_s:
        case SDLK_t:
        case SDLK_u:
        case SDLK_v:
        case SDLK_w:
        case SDLK_x:
        case SDLK_y:
        case SDLK_z:
            inputCharacter(input, key);
            break;
    }
}

void TuiInputDestroy(TuiInput *input) {
    StringDestroy(input->text);
    free(input);
}

static void inputCharacter(TuiInput *input, char value) {
    if (StringSize(input->text) >= input->width) {
        return;
    }

    // If at end, append
    if (input->cursor_x == StringSize(input->text)) {
        StringAppend(input->text, value);
        input->cursor_x++;
        return;
    }
    // else, insert
    StringInsert(input->text, input->cursor_x++, value);
}

static void backspaceText(TuiInput *input) {
    if (input->cursor_x <= 0) {
        return;
    }

    // Must shift all characters otherwise they disappear
    for (int i = input->cursor_x; i < StringSize(input->text); i++) {
        StringSetAt(input->text, i, StringGetAt(input->text, i + 1));
    }

    // Make sure to remove the last character
    StringRemoveAt(input->text, StringSize(input->text) - 1);

    input->cursor_x--;
}