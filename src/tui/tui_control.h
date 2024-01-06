#ifndef TUI_CONTROL_H
#define TUI_CONTROL_H

#define TUI_CONTROL_WINDOW 0
#define TUI_CONTROL_LABEL 1
#define TUI_CONTROL_INPUT 2

#include "../txtsdl_screen.h"
#include "../txtsdl_events.h"

typedef struct _TuiControl {
    int type;
    void *control;
} TuiControl;

TuiControl *TuiControlCreate(int type, void *control);
void TuiControlDraw(TuiControl *control, TxtSDLScreen *screen);
void TuiControlKeyPress(TuiControl *control, TxTSDLKeyEvent *event);
void TuiControlDestroy(TuiControl *control);

#endif