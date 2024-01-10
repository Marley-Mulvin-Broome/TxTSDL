#include "txtsdl.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "txtsdl_screen.h"
#include "txtsdl_constants.h"
#include "txtsdl_events.h"
#include "txtsdl_cursor.h"
#include "ds/list.h"

static bool hasInitiatialised = false;
SDL_Window *TxtSDL_Window;
SDL_Renderer *TxtSDL_Renderer;
TxtSDLScreen *TxtSDL_Screen;
TxtSDLCursor *TxtSDL_Cursor;
List *TxtSDL_KeyPressEventHandlers;

static void assertInitialised(const char *message);
static void cleanupEventHandlers(void);

void TxtSDL_Run(
    const TxtSDL_WindowInfo *window_info, 
    const TxtSDL_FontLoadInfo *font_info,
    TxtSDL_UpdateFunction update,
    TxtSDL_SetupFunction setup,
    TxtSDL_DrawFunction draw
) {
    TxtSDLScreen *screen = TxtSDL_Init(
        window_info,
        font_info
    );

    if (setup) {
        setup(screen);
    }

    SDL_Event event;
    bool game_running = true;

    float current_time = 0;
    float prev_time = 0;
    float delta_time = 0;

    TxtSDL_SetCursorVisible(true);

    while (game_running)
    {
        prev_time = current_time;
        current_time = SDL_GetTicks();
        delta_time = (current_time - prev_time) / 1000.0f;
        
        TxtSDLScreen_Clear(screen);

        // Poll for events

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    game_running = false;
                    break;

                case SDL_KEYDOWN:
                    TxTSDLKeyEvent key_event = {
                        .key = event.key.keysym.sym,
                        .modifiers = event.key.keysym.mod,
                        .type = TXTSDL_KEYDOWN,
                    };

                    for (int i = 0; i < ListSize(TxtSDL_KeyPressEventHandlers); i++) {
                        TxtSDL_KeyPressEventHandlerWrapper *wrapper = ListGet(TxtSDL_KeyPressEventHandlers, i);
                        wrapper->handler(&key_event, wrapper->data);
                    }
                    break;

                default:
                    break;
            }
        }

        if (update) {
            update(screen, delta_time);
        }

        TxtSDL_StartRender();

        if (draw) {
            draw(screen);
        }

        TxtSDLScreen_DrawBuffer(screen);
        TxtSDLCursor_Draw(
            TxtSDL_Cursor, 
            TxtSDLScreen_GetCellWidth(TxtSDL_Screen), 
            TxtSDLScreen_GetCellHeight(TxtSDL_Screen)
        );

        TxtSDL_UpdateWindow();
    }

    TxtSDL_Cleanup();
	TxtSDL_Quit();
}


TxtSDLScreen *TxtSDL_Init(
    const TxtSDL_WindowInfo *window_info,
    const TxtSDL_FontLoadInfo *font_info
) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

    TxtSDL_Window = TxtSDL_CreateWindow(
        window_info->title, 
        window_info->x, 
        window_info->y, 
        window_info->width,
        window_info->height,
        window_info->flags
    );

    TxtSDL_Renderer = SDL_CreateRenderer(
        TxtSDL_Window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    TxtSDL_Screen = TxtSDLScreen_Create(
        50,
        50,
        font_info->target_width,
        font_info->target_height,
        font_info->font_img_path,
        font_info->font_descriptor_path
    );

    TxtSDL_Cursor = TxtSDLCursor_Create(
        0,
        0,
        16,
        4
    );

    TxtSDL_KeyPressEventHandlers = ListCreate();

    hasInitiatialised = true;

    return TxtSDL_Screen;
}

/**
 * Creates a window
*/
SDL_Window *TxtSDL_CreateWindow(const char *title, int x, int y, int width, int height, Uint32 flags) {
	SDL_Window *window = SDL_CreateWindow(title, x, y, width, height, flags);

	if (!window) {
		fprintf(stderr, "Failed to initialise window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	return window;
}

SDL_Texture *TxtSDL_LoadTexture(const char *image_path) {
    SDL_Surface *image_surface = SDL_LoadBMP(image_path);

    if (!image_surface) {
        fprintf(stderr, "Failed loading image (%s): %s\n", image_path, SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(
        TxtSDL_Renderer, 
        image_surface
    );

    if (!texture) {
        fprintf(stderr, "Failed loading texture (%s): %s.\n", image_path, SDL_GetError());
    }

    SDL_FreeSurface(image_surface);

    return texture;
}

void TxtSDL_StartRender(void) {
    SDL_RenderClear(TxtSDL_Renderer);
}

void TxtSDL_SetDrawColour(const TxtSDLColour *colour) {
    SDL_SetRenderDrawColor(
        TxtSDL_Renderer,
        colour->r,
        colour->g,
        colour->b,
        255
    );
}

void TxtSDL_DrawRect(const SDL_Rect *rect, bool filled) {
    SDL_RenderDrawRect(TxtSDL_Renderer, rect);

    if (filled) {
        SDL_RenderFillRect(
            TxtSDL_Renderer,
            rect
        );
    }
}

void TxtSDL_DrawImage(
    const SDL_Rect *source_rect,
    const SDL_Rect *target_rect, 
    SDL_Texture *texture
) {
    int result = SDL_RenderCopy(TxtSDL_Renderer, texture, source_rect, target_rect);

    if (result != TXTSDL_SUCCESS) {
        fprintf(stderr, "Error drawing image to the screen with SDL_RenderCopy: %s\n", SDL_GetError());
    }
}

void TxtSDL_SetTextureColour(
    SDL_Texture *texture, 
    const TxtSDLColour *colour
) {
    int result = SDL_SetTextureColorMod(
        texture, 
        colour->r, 
        colour->g, 
        colour->b
    );

    if (result != TXTSDL_SUCCESS) {
        fprintf(stderr, "Error setting texture colour: %s\n", SDL_GetError());
    }
}

void TxtSDL_SetCursorVisible(bool visible) {
    TxtSDLCursor_SetVisible(
        TxtSDL_Cursor,
        visible
    );
}

void TxtSDL_SetCursor(int x, int y) {
    TxtSDLCursor_SetPosition(
        TxtSDL_Cursor,
        x,
        y
    );
}

void TxtSDL_UpdateWindow(void) {
    assertInitialised(
        "Cannot update window without TxtSDL being initialised!"
    );

    SDL_RenderPresent(TxtSDL_Renderer);
}

void TxtSDL_Cleanup(void) {
    cleanupEventHandlers();
    TxtSDLScreen_Destroy(TxtSDL_Screen);
    SDL_DestroyWindow(TxtSDL_Window);
    SDL_DestroyRenderer(TxtSDL_Renderer);
}

void TxtSDL_Quit(void) {
    SDL_Quit();
}

// Event handling

void TxtSDL_AddKeyPressEventHandler(TxtSDL_KeyPressEventHandler handler, void *data) {
    TxtSDL_KeyPressEventHandlerWrapper *wrapper = malloc(sizeof(TxtSDL_KeyPressEventHandlerWrapper));

    wrapper->data = data;
    wrapper->handler = handler;

    ListAdd(TxtSDL_KeyPressEventHandlers, wrapper);
}

void TxtSDL_RemoveKeyPressEventHandler(void (*handler)(TxTSDLKeyEvent *)) {
    // TODO: Deal with wrapper, this doesn't currently work properly
    ListRemove(TxtSDL_KeyPressEventHandlers, handler);
}


static void assertInitialised(const char *message) {
    if (!hasInitiatialised) {
        fprintf(stderr, "%s\n", message);
        exit(EXIT_FAILURE);
    }
}

static void cleanupEventHandlers(void) {
    for (int i = 0; i < ListSize(TxtSDL_KeyPressEventHandlers); i++) {
        free(ListGet(TxtSDL_KeyPressEventHandlers, i));
    }
    ListDestroy(TxtSDL_KeyPressEventHandlers);
}