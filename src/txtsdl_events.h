#ifndef TXTSDL_EVENTS_H
#define TXTSDL_EVENTS_H

#include <stdint.h>

#define TXTSDL_KEYDOWN 1
#define TXTSDL_KEYUP 2
#define TXTSDL_SUBMIT 3


/**
 * @brief Represents a key event.
*/
typedef struct _TxTSDLKeyEvent {
    int type;
    int key;
    uint16_t modifiers;
} TxTSDLKeyEvent;

typedef struct _TxtSDLSubmitEvent {
    int type;
} TxtSDLSubmitEvent;

/**
 * Function called when a key is pressed.
*/
typedef void (*TxtSDL_KeyPressEventHandler)(
    TxTSDLKeyEvent *event,
    void *data
);

typedef void (*TxtSDL_SubmitEventHandler)(
    TxtSDLSubmitEvent *event,
    void *data
);

/**
 * Represents a rapper with aditional data to pass to the event handler.
*/
typedef struct _TxtSDLKeyEventWrapper {
    TxtSDL_KeyPressEventHandler handler;
    void *data;
} TxtSDL_KeyPressEventHandlerWrapper;

typedef struct _TxtSDLSubmitEventHandlerWrapper {
    TxtSDL_SubmitEventHandler handler;
    void *data;
} TxtSDL_SubmitEventHandlerWrapper;

#endif