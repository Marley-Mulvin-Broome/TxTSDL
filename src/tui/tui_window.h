#ifndef TUI_WINDOW_H
#define TUI_WINDOW_H

#include "tui_control.h"
#include "../txtsdl_screen.h"
#include "../txtsdl_events.h"

typedef struct _TuiWindow TuiWindow;

/**
 * Creates a new TUI window.
 * @param title The title of the window.
 * @param x The x position of the window.
 * @param y The y position of the window.
 * @param width The width of the window.
 * @param height The height of the window.
 * @param screen The screen to draw the window on.
 * @return The newly created window.
*/
TuiWindow *TuiWindowCreate(
    const char *title, int x, int y, int width, int height, TxtSDLScreen *screen
);

/**
 * Draws a TUI window.
 * @param window The window to draw.
*/
void TuiWindowDraw(TuiWindow *window);

/**
 * Adds a child to a TUI window.
 * @param window The window to add the child to.
 * @param child The child to add to the window.
*/
void TuiWindowAddChild(TuiWindow *window, TuiControl *child);

/**
 * Handles a key press event for a TUI window.
 * @param window The window to handle the event for.
 * @param event The event to handle.
*/
void TuiWindowKeyPress(TuiWindow *window, TxTSDLKeyEvent *event);

/**
 * Destroys a TUI window and all child components.
 * @param window The window to destroy.
*/
void TuiWindowDestroy(TuiWindow *window);

#endif