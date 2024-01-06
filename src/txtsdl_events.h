#ifndef TXTSDL_EVENTS_H
#define TXTSDL_EVENTS_H

#include <stdint.h>

#define TXTSDL_KEYDOWN 1

typedef struct _TxTSDLKeyEvent {
    int type;
    int key;
    uint16_t modifiers;
} TxTSDLKeyEvent;

#endif