#include "tui_label.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../txtsdl_screen.h"
#include "../txtsdl_colour.h"

typedef struct _TuiLabel {
    int x, y;
    String *text;
} TuiLabel;

TuiLabel *TuiLabelCreate(int x, int y, const char *text) {
    TuiLabel *label = malloc(sizeof(TuiLabel));

    if (!label) {
        fprintf(stderr, "Failed to allocate memory for label\n");
        return NULL;
    }

    label->text = StringFromCString(text);

    if (!label->text) {
        fprintf(stderr, "Failed to allocate memory for label text\n");
        return NULL;
    }

    label->x = x;
    label->y = y;

    return label;
}

void TuiLabelDraw(TuiLabel *label, TxtSDLScreen *screen) {
    TxtSDLScreen_WriteString(
        screen,
        label->x,
        label->y,
        label->text,
        &COLOUR_BLACK,
        &COLOUR_WHITE,
        false
    );
}

void TuiLabelDestroy(TuiLabel *label) {
    StringDestroy(label->text);
    free(label);
}