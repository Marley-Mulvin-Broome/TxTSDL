#ifndef TXTSDL_CHAR_CELL_H
#define TXTSDL_CHAR_CELL_H

#include <stdint.h>

#include "txtsdl_colour.h"

/**
 * Represents the data stored in each position of a TxtSDL_Screen
*/
typedef struct _TxtSDLCharCell {
    char value;
    TxtSDLColour foreground;
    TxtSDLColour background;
} TxtSDLCharCell;

#endif