#include "tui_control.h"

#include <stdio.h>
#include <stdlib.h>

#include "tui_label.h"
#include "tui_window.h"
#include "tui_input.h"

#include "../txtsdl_screen.h"
#include "../txtsdl_events.h"

TuiControl *TuiControlCreate(int type, void *control) {
    TuiControl *tuiControl = malloc(sizeof(TuiControl));

    if (!tuiControl) {
        fprintf(stderr, "Failed to allocate memory for TuiControl\n");
        return NULL;
    }

    tuiControl->type = type;
    tuiControl->control = control;

    return tuiControl;
}

void TuiControlDraw(TuiControl *control, TxtSDLScreen *screen) {
    switch (control->type) {
        case TUI_CONTROL_LABEL:
            TuiLabelDraw(control->control, screen);
            break;
        case TUI_CONTROL_WINDOW:
            TuiWindowDraw(control->control);
            break;
        case TUI_CONTROL_INPUT:
            TuiInputDraw(control->control);
            break;
        default:
            fprintf(stderr, "Unknown control type: %d\n", control->type);
            break;
    }
}

void TuiControlKeyPress(TuiControl *control, TxTSDLKeyEvent *event) {
    switch (control->type) {
        case TUI_CONTROL_INPUT:
            TuiInputKeyPress(control->control, event->key);
            break;
        case TUI_CONTROL_WINDOW:
            TuiWindowKeyPress(control->control, event);
            break;
        case TUI_CONTROL_LABEL:
        // Insert all other non keypress controls here
            break;
        default:
            fprintf(stderr, "Unknown control type: %d\n", control->type);
            break;
    }

}

void TuiControlDestroy(TuiControl *control) {
    switch (control->type) {
        case TUI_CONTROL_LABEL:
            TuiLabelDestroy(control->control);
            break;
        case TUI_CONTROL_WINDOW:
            TuiWindowDestroy(control->control);
            break;
        case TUI_CONTROL_INPUT:
            TuiInputDestroy(control->control);
            break;
        default:
            fprintf(stderr, "Unknown control type: %d\n", control->type);
            break;
    }

    free(control);
}