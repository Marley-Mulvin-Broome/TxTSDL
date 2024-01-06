#ifndef TUI_WINDOW_H
#define TUI_WINDOW_H

#include "tui_control.h"
#include "../txtsdl_screen.h"
#include "../txtsdl_events.h"

typedef struct _TuiWindow TuiWindow;

TuiWindow *TuiWindowCreate(
    const char *title, int x, int y, int width, int height, TxtSDLScreen *screen
);
void TuiWindowDraw(TuiWindow *window);
void TuiWindowAddChild(TuiWindow *window, TuiControl *child);
void TuiWindowKeyPress(TuiWindow *window, TxTSDLKeyEvent *event);
void TuiWindowDestroy(TuiWindow *window);

#endif