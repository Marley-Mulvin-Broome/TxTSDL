#ifndef TUI_LABEL_H
#define TUI_LABEL_H

#include "../txtsdl_screen.h"

typedef struct _TuiLabel TuiLabel;

TuiLabel *TuiLabelCreate(int x, int y, const char *text);
void TuiLabelDraw(TuiLabel *label, TxtSDLScreen *screen);
void TuiLabelDestroy(TuiLabel *label);

#endif