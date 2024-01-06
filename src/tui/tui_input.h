#ifndef TUI_INPUT_H
#define TUI_INPUT_H

#include "../txtsdl_screen.h"

typedef struct _TuiInput TuiInput;

TuiInput *TuiInputCreate(int x, int y, int width, TxtSDLScreen *screen);
void TuiInputDraw(TuiInput *input);
void TuiInputKeyPress(TuiInput *input, int key);
void TuiInputDestroy(TuiInput *input);

#endif