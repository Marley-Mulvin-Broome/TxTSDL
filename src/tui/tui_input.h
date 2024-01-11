#ifndef TUI_INPUT_H
#define TUI_INPUT_H

#include "../txtsdl_events.h"
#include "../txtsdl_screen.h"

typedef struct _TuiInput TuiInput;

/**
 * Creates a new TUI input.
 * @param x The x position of the input.
 * @param y The y position of the input.
 * @param width The width of the input.
 * @param screen The screen to draw the input on.
 * @return The newly created input.
*/
TuiInput *TuiInputCreate(int x, int y, int width, TxtSDLScreen *screen);

/**
 * Draws a TUI input.
 * @param input The input to draw.
*/
void TuiInputDraw(TuiInput *input);

void TuiInputFocus(TuiInput *input);
void TuiInputUnfocus(TuiInput *input);

bool TuiInputEnter(TuiInput *input);

/**
 * Handles a key press event for a TUI input.
 * @param input The input to handle the event for.
 * @param key The key to handle.
*/
void TuiInputKeyPress(TuiInput *input, TxTSDLKeyEvent *key);

/**
 * Destroys a TUI input.
 * @param input The input to destroy.
*/
void TuiInputDestroy(TuiInput *input);

#endif