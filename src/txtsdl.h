#ifndef TXTSDL_H
#define TXTSDL_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>

#include "txtsdl_screen.h"
#include "txtsdl_colour.h"
#include "txtsdl_events.h"
#include "txtsdl_events.h"

/**
 * Function which is called every frame to update the game state
 * @param screen The screen to draw to
 * @param delta_time The time since the last frame in seconds
*/
typedef void (*TxtSDL_UpdateFunction)(
    TxtSDLScreen *screen, float delta_time
);

/**
 * Function which is called once before the game loop starts
 * @param screen The screen to draw to
*/
typedef void (*TxtSDL_SetupFunction)(
    TxtSDLScreen *screen
);

/**
 * Information about a window to create
 * @param title The title of the window
 * @param x The x position of the window
 * @param y The y position of the window
 * @param width The width of the window
 * @param height The height of the window
 * @param flags The flags to pass to SDL_CreateWindow
*/
typedef struct {
    char *title;
    int x;
    int y;
    int width;
    int height;
    Uint32 flags;
} TxtSDL_WindowInfo;

/**
 * Runs the TxtSDL engine game loop. This function will also initialise the engine.
 * This function doesn't have to be used, but is an easy way to get started.
 * @param window_info The information about the window to create
 * @param update The function to call every frame to update the game state
 * @param setup The function to call once before the game loop starts
 * @note This function will not return until the game is quit
*/
void TxtSDL_Run(
    const TxtSDL_WindowInfo *window_info, 
    TxtSDL_UpdateFunction update,
    TxtSDL_SetupFunction setup
);

/**
 * Initialises the TxtSDL engine and the screen to draw to
 * @param window_info The information about the window to create
*/
TxtSDLScreen *TxtSDL_Init(const TxtSDL_WindowInfo *window_info);

/**
 * Creates a window for rendering to
 * @param title The title of the window
 * @param x The x position of the window
 * @param y The y position of the window
 * @param width The width of the window
 * @param height The height of the window
 * @param flags The flags to pass to SDL_CreateWindow
*/
SDL_Window *TxtSDL_CreateWindow(const char *title, int x, int y, int width, int height, Uint32 flags);

/**
 * Loads a texture from a bitmap image
 * @param image_path The path to the image
*/
SDL_Texture *TxtSDL_LoadTexture(const char *image_path);

/**
 * Resets the render buffer to its initial state for a new frame
*/
void TxtSDL_StartRender(void);

/**
 * Sets the draw colour for rendering like shapes, NOT images.
 * @param colour The colour to set
*/
void TxtSDL_SetDrawColour(const TxtSDLColour *colour);

/**
 * Draws a rectangle to the screen
 * @param rect The rectangle to draw
 * @param filled Whether or not the rectangle should be filled
*/
void TxtSDL_DrawRect(const SDL_Rect *rect, bool filled);

/**
 * Draws an image to the screen
 * @param source_rect The space on the image to draw from
 * @param target_rect The space on the screen to draw to
 * @param texture The texture to draw
*/
void TxtSDL_DrawImage(
    const SDL_Rect *source_rect,
    const SDL_Rect *target_rect, 
    SDL_Texture *texture
);

/**
 * Changes the colour of a texture
 * @param texture The texture to change the colour of
 * @param colour The colour to change the texture to
*/
void TxtSDL_SetTextureColour(
    SDL_Texture *texture, 
    const TxtSDLColour *colour
);

/**
 * Sets the cursor to be visible or not
 * @param visible Whether or not the cursor should be visible
*/
void TxtSDL_SetCursorVisible(bool visible);

/**
 * Sets the position of the cursor
 * @param x The x position of the cursor
 * @param y The y position of the cursor
*/
void TxtSDL_SetCursor(int x, int y);

/**
 * Draws the render buffer to the screen, making any drawn changes visible
*/
void TxtSDL_UpdateWindow(void);

/**
 * Cleansup all the resources used internally by TxtSDL
*/
void TxtSDL_Cleanup(void);

/**
 * Quits TxtSDL
*/
void TxtSDL_Quit(void);

// Event handling

/**
 * Adds a function to be called when a key is pressed
 * @param handler The function to call
 * @param data The data to pass to the function
 * @note This function only works when using TxtSDL_Run
*/
void TxtSDL_AddKeyPressEventHandler(TxtSDL_KeyPressEventHandler handler, void *data);

// TODO: Fix this function to work
void TxtSDL_RemoveKeyPressEventHandler(void (*handler)(TxTSDLKeyEvent *));

#endif