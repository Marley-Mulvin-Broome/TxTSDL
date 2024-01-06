#ifndef TXTSDL_CHAR_CELL_H
#define TXTSDL_CHAR_CELL_H

#include <stdint.h>

#include "txtsdl_colour.h"

typedef struct _TxtSDLCharCell {
    char value;
    TxtSDLColour foreground;
    TxtSDLColour background;
} TxtSDLCharCell;

#endif