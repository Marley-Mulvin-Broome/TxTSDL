#ifndef TUI_CONTROL_H
#define TUI_CONTROL_H

#define TUI_CONTROL_WINDOW 0
#define TUI_CONTROL_LABEL 1
#define TUI_CONTROL_INPUT 2

#include "../txtsdl_screen.h"
#include "../txtsdl_events.h"

// TODO: Make TuiControl opaque

/**
 * A generic struct for containing any TUI control, keeping track of their type.
*/
typedef struct _TuiControl {
    int type;
    void *control;
} TuiControl;

/**
 * Creates a new TUI control.
 * @param type The type of the control.
 * @param control The control to contain.
 * @return The newly created control.
*/
TuiControl *TuiControlCreate(int type, void *control);

/**
 * Draws a TUI control.
 * @param control The control to draw.
 * @param screen The screen to draw the control on.
*/
void TuiControlDraw(TuiControl *control, TxtSDLScreen *screen);

/**
 * Checks if a TUI control is focusable.
 * @param control The control to check.
 * @return Whether or not the control is focusable.
*/
bool TuiControlFocusable(TuiControl *control);

/**
 * Focuses a TUI control.
 * @param control The control to focus.
*/
void TuiControlFocus(TuiControl *control);

void TuiControlUnfocus(TuiControl *control);

bool TuiControlEnter(TuiControl *control);

/**
 * Handles a key press event for a TUI control.
 * @param control The control to handle the event for.
 * @param event The event to handle.
*/
void TuiControlKeyPress(TuiControl *control, TxTSDLKeyEvent *event);

/**
 * Destroys a TUI control.
 * @param control The control to destroy.
*/
void TuiControlDestroy(TuiControl *control);

#endif