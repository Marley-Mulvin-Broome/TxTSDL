#ifndef TUI_LABEL_H
#define TUI_LABEL_H

#include "../txtsdl_screen.h"

typedef struct _TuiLabel TuiLabel;

/**
 * Creates a new TUI label.
 * @param x The x position of the label.
 * @param y The y position of the label.
 * @param text The text of the label.
*/
TuiLabel *TuiLabelCreate(int x, int y, const char *text);

/**
 * Draws a TUI label.
 * @param label The label to draw.
 * @param screen The screen to draw the label on.
*/
void TuiLabelDraw(TuiLabel *label, TxtSDLScreen *screen);

/**
 * Destroys a TUI label.
 * @param label The label to destroy.
*/
void TuiLabelDestroy(TuiLabel *label);

#endif