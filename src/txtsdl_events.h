#ifndef TXTSDL_EVENTS_H
#define TXTSDL_EVENTS_H

#include <stdint.h>

#define TXTSDL_KEYDOWN 1


/**
 * @brief Represents a key event.
*/
typedef struct _TxTSDLKeyEvent {
    int type;
    int key;
    uint16_t modifiers;
} TxTSDLKeyEvent;

/**
 * Function called when a key is pressed.
*/
typedef void (*TxtSDL_KeyPressEventHandler)(
    TxTSDLKeyEvent *event,
    void *data
);

/**
 * Represents a rapper with aditional data to pass to the event handler.
*/
typedef struct _TxtSDLKeyEventWrapper {
    TxtSDL_KeyPressEventHandler handler;
    void *data;
} TxtSDL_KeyPressEventHandlerWrapper;

#endif