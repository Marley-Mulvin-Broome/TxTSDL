#ifndef TXTSDL_COLOUR_H
#define TXTSDL_COLOUR_H

#define COLOUR_WHITE (TxtSDLColour){ .r = 255, .g = 255, .b = 255 }
#define COLOUR_BLACK (TxtSDLColour){ .r = 0, .g = 0, .b = 0 }

#include <stdint.h>

typedef struct _TxtSDLColour {
    uint8_t r, g, b;
} TxtSDLColour;

#endif