#ifndef TXTSDL_CURSOR_H
#define TXTSDL_CURSOR_H

#include <stdbool.h>

typedef struct _TxtSDLCursor TxtSDLCursor;

TxtSDLCursor *TxtSDLCursor_Create(int x, int y, int width, int height);
void TxtSDLCursor_SetPosition(TxtSDLCursor *cursor, int x, int y);
void TxtSDLCursor_SetVisible(TxtSDLCursor *cursor, bool visible);
void TxtSDLCursor_Draw(TxtSDLCursor *cursor, int cell_width, int cell_height);
void TxtSDLCursor_Destroy(TxtSDLCursor *cursor);


#endif