#include "txtsdl_screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "txtsdl.h"
#include "txtsdl_colour.h"
#include "txtsdl_constants.h"
#include "ds/hashmap.h"

// ===== Private Fields ===== //
typedef struct _Screen {
    TxtSDLCharCell **buffer;
    int width;
    int height;
    int cell_width;
    int cell_height;
    Hashmap *char_to_rect_map;
    SDL_Texture *charset_texture;
} TxtSDLScreen;

static const char TxtSDL_SupportedCharacters[] = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    ' '
};

static TxtSDLCharCell **createBuffer(int width, int height);
static void setupDefaultCharCell(TxtSDLCharCell *cell);
static void freeBuffer(TxtSDLCharCell **buffer, int width);
static void drawBufferCell(TxtSDLScreen *screen, int x, int y);
static Hashmap *createCharRectHashmap();
static void destroyCharRectHashmap(Hashmap *hashmap);

// ===== Interface ===== //


TxtSDLScreen *TxtSDLScreen_Create(
    int width, 
    int height, 
    int cell_width,
    int cell_height,
    const char *charset_img
) {
    TxtSDLScreen *screen = malloc(sizeof(TxtSDLScreen));

    if (!screen) {
        fprintf(stderr, "Error creating TxtSDLScreen.\n");
        return NULL;
    }

    screen->buffer = createBuffer(width, height);
    screen->width = width;
    screen->height = height;
    screen->cell_width = cell_width;
    screen->cell_height = cell_height;
    screen->charset_texture = TxtSDL_LoadTexture(charset_img);
    screen->char_to_rect_map = createCharRectHashmap(
        screen->cell_width,
        screen->cell_height
    );

    return screen;
}

void TxtSDLScreen_WriteChar(TxtSDLScreen *screen, int x, int y, const TxtSDLCharCell *cell_value) {
    if (!TxtSDLScreen_XInBound(screen, x)) {
        fprintf(stderr, "Cannot write character with x %d, out of bounds", x);
        return;
    }

    if (!TxtSDLScreen_YInBound(screen, y)) {
        fprintf(stderr, "Cannot write character with y %d, out of bounds", y);
        return;
    }

    if (!HashmapContainsKey(screen->char_to_rect_map, cell_value->value)) {
        fprintf(stderr, "Cannot write unsupported character '%c'\n", cell_value->value);
        return;
    }

    screen->buffer[x][y].value = cell_value->value;
    screen->buffer[x][y].foreground = cell_value->foreground;
    screen->buffer[x][y].background = cell_value->background;
}

int TxtSDLScreen_WriteString(
    TxtSDLScreen *screen, 
    int x, 
    int y, 
    const char *text,
    const TxtSDLColour *foreground,
    const TxtSDLColour *background,
    bool overflow
) {
    if (!TxtSDLScreen_PointInBounds(screen, x, y)) {
        fprintf(stderr, "Cannot print string to point out of bounds (%d, %d)\n", x, y);
        return TXTSDL_FAILURE;
    }

    int length = strlen(text);

    if (length == 0) {
        return TXTSDL_SUCCESS;
    }

    TxtSDLCharCell cell_value = {
        .foreground = *foreground,
        .background = *background,
        .value = ' '
    };

    for (int i = 0; i < length; i++, x++) {
        cell_value.value = text[i];

        // Must check if we write out of width bounds or error
        if (!TxtSDLScreen_XInBound(screen, x)) {
            if (!overflow) {
                return TXTSDL_SUCCESS;
            }

            x = 0; 
            y++;
        }

        TxtSDLScreen_WriteChar(screen, x, y, &cell_value);
    }

    return TXTSDL_SUCCESS;    
}

void TxtSDLScreen_Clear(TxtSDLScreen *screen) {
    for (int x = 0; x < screen->width; x++) {
        for (int y = 0; y < screen->height; y++) {
            setupDefaultCharCell(&(screen->buffer[x][y]));
        }
    }
}


void TxtSDLScreen_DrawBuffer(TxtSDLScreen *screen) {
    TxtSDL_StartRender();

    for (int x = 0; x < screen->width; x++) {
        for (int y = 0; y < screen->height; y++) {
            drawBufferCell(screen, x, y);
        }
    }

    TxtSDL_UpdateWindow();
}

bool TxtSDLScreen_PointInBounds(const TxtSDLScreen *screen, int x, int y) {
    return (
        TxtSDLScreen_XInBound(screen, x) && 
        TxtSDLScreen_YInBound(screen, y)
    );
}

bool TxtSDLScreen_YInBound(const TxtSDLScreen *screen, int y) {
    return y >= 0 && y < screen->height;
}

bool TxtSDLScreen_XInBound(const TxtSDLScreen *screen, int x) {
    return x >= 0 && x < screen->width;
}

void TxtSDLScreen_Destroy(TxtSDLScreen *screen) {
    destroyCharRectHashmap(screen->char_to_rect_map);
    freeBuffer(screen->buffer, screen->width);
    free(screen);
}


// ===== Private implementations ===== //

/**
 * Creates a two dimensional array of characters with the specified dimensions
*/
static TxtSDLCharCell **createBuffer(int width, int height) {
    TxtSDLCharCell **buffer = malloc(sizeof(TxtSDLCharCell*) * width);

    if (!buffer) {
        fprintf(stderr, "Error creating width buffer for TxtSDLScreen.\n");
        return NULL;
    }

    for (int i = 0; i < width; i++) {
        buffer[i] = malloc(sizeof(TxtSDLCharCell) * height);

        if (!buffer[i]) {
            fprintf(stderr, "Error creating height buffer for TxtSDLScreen.\n");
            return NULL;
        }

        for (int j = 0; j < height; j++) {
            setupDefaultCharCell(&(buffer[i][j]));
        }
    }

    return buffer;
}

static void setupDefaultCharCell(TxtSDLCharCell *cell) {
    cell->value = ' ';
    cell->background = COLOUR_BLACK;
    cell->foreground = COLOUR_WHITE;
}

static void freeBuffer(TxtSDLCharCell **buffer, int width) {
    for (int i = 0; i < width; i++) {
        free(buffer[i]);
    }

    free(buffer);
}

static void drawBufferCell(TxtSDLScreen *screen, int x, int y) {
    TxtSDLCharCell cell = screen->buffer[x][y];

    SDL_Rect drawing_rect = {
        .x = x * screen->cell_width,
        .y = y * screen->cell_height,
        .w = screen->cell_width,
        .h = screen->cell_height
    };

    SDL_Rect *source_rect = (SDL_Rect *)HashmapGet(
        screen->char_to_rect_map,
        cell.value
    )->data;

    // Drawing background first or will appear on-top of text
    TxtSDL_SetDrawColour(
        &(cell.background)
    );

    TxtSDL_DrawRect(&drawing_rect, true);
    
    // Drawing foreground so text appears above background
    TxtSDL_SetTextureColour(
        screen->charset_texture,
        &(cell.foreground)
    );

    TxtSDL_DrawImage(
        source_rect,
        &drawing_rect,
        screen->charset_texture
    );
}

static Hashmap *createCharRectHashmap(int cell_width, int cell_height) {
    Hashmap *hashmap = HashmapCreate();

    for (int i = 0; i < sizeof(TxtSDL_SupportedCharacters); i++) {
        SDL_Rect *rect = malloc(sizeof(SDL_Rect));

        rect->w = cell_width;
        rect->h = cell_height;
        rect->x = cell_width * i;
        rect->y = 0;
        
        HashmapInsert(
            hashmap,
            TxtSDL_SupportedCharacters[i],
            rect
        );
    }

    return hashmap;
}

static void destroyCharRectHashmap(Hashmap *hashmap) {
    // TODO: destroy all the items
    HashmapDestroy(hashmap);
}