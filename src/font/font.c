#include "font.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "../txtsdl.h"
#include "../txtsdl_constants.h"
#include "../ds/hashmap.h"
#include "../ds/list.h"
#include "../ds/charlist.h"

typedef struct _TxtSDLFont {
    String *name;
    int glyph_width, glyph_height;
    SDL_Texture *texture;
    Hashmap *glyph_rects;
} TxtSDLFont;

static Hashmap *createGlyphRectHashmap();
static void destroyGlyphRectHashmap(Hashmap *hashmap);
static int loadDescriptor(TxtSDLFont *font, const char *descriptor_path);
static int parseFontMetadata(TxtSDLFont *font, FILE *descriptor);
static int parseDescriptor(TxtSDLFont *font, FILE *descriptor);
static int characterOrderListToRect(TxtSDLFont *font, List *character_order);


TxtSDLFont *TxtSDLFont_Create(
    const char *name,
    int glyph_width,
    int glyph_height,
    const char *image_path,
    const char *descriptor_path
) {
    TxtSDLFont *font = malloc(sizeof(TxtSDLFont));

    if (!font) {
        fprintf(stderr, "Failed to allocate memory for font\n");
        return NULL;
    }

    font->name = StringFromCString(name);

    if (!font->name) {
        fprintf(stderr, "Failed to allocate memory for font name\n");
        TxtSDLFont_Destroy(font);
        return NULL;
    }

    font->glyph_width = glyph_width;
    font->glyph_height = glyph_height;
    font->glyph_rects = createGlyphRectHashmap();

    if (!font->glyph_rects) {
        fprintf(stderr, "Failed to create glyph rects hashmap\n");
        TxtSDLFont_Destroy(font);
        return NULL;
    }

    font->texture = TxtSDL_LoadTexture(image_path);

    if (!font->texture) {
        fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
        TxtSDLFont_Destroy(font);
        return NULL;
    }

    int result = loadDescriptor(font, descriptor_path);

    if (result != TXTSDL_SUCCESS) {
        fprintf(stderr, "Failed to load font descriptor\n");
        TxtSDLFont_Destroy(font);
        return NULL;
    }

    return font;
}

TxtSDLCharacterDrawInfo TxtSDLFont_GetCharacter(TxtSDLFont *font, uchar_t character) {
    SDL_Rect *rect = HashmapGet(font->glyph_rects, character)->data;

    if (!rect) {
        fprintf(stderr, "Failed to get glyph rect for character: %c\n", character);
        return (TxtSDLCharacterDrawInfo) {
            NULL,
            NULL
        };
    }

    return (TxtSDLCharacterDrawInfo) {
        rect,
        font->texture
    };
}

bool TxtSDLFont_ContainsCharacter(TxtSDLFont *font, uchar_t character) {
    return HashmapContainsKey(font->glyph_rects, character);
}

void TxtSDLFont_Destroy(TxtSDLFont *font) {
    if (!font) {
        return;
    }

    if (font->name) {
        free(font->name);
    }

    if (font->glyph_rects) {
        destroyGlyphRectHashmap(font->glyph_rects);
    }

    if (font->texture) {
        SDL_DestroyTexture(font->texture);
    }

    free(font);
}

static Hashmap *createGlyphRectHashmap() {
    Hashmap *hashmap = HashmapCreate();

    if (!hashmap) {
        fprintf(stderr, "Failed to create glyph rect hashmap\n");
        return NULL;
    }

    return hashmap;

}

static void destroyGlyphRectHashmap(Hashmap *hashmap) {
    HashmapIterate(hashmap, free);
    HashmapDestroy(hashmap);
}

static int loadDescriptor(TxtSDLFont *font, const char *descriptor_path) {
    FILE *descriptor = fopen(descriptor_path, "r");

    if (!descriptor) {
        fprintf(stderr, "Failed to open font descriptor: %s\n", strerror(errno));
        return TXTSDL_FAILURE;
    }

    int result = parseDescriptor(font, descriptor);

    if (result != TXTSDL_SUCCESS) {
        fprintf(stderr, "Failed to parse font descriptor\n");
    }

    fclose(descriptor);

    return result;
}

static int parseFontMetadata(TxtSDLFont *font, FILE *descriptor) {
    String *current_line;

    while ((current_line = StringFromFileDescriptorLine(descriptor)) != NULL) {
        if (StringCompareCString(current_line, FONT_PARSE_END_TAG)) {
            StringDestroy(current_line);
            return TXTSDL_SUCCESS;
        }
        
        if (!StringContains(current_line, '=')) {
            fprintf(stderr, "Invalid metadata line: ");
            StringWriteToFileDescriptor(current_line, stderr);
            fprintf(stderr, "\n");
            StringDestroy(current_line);
            return TXTSDL_FAILURE;
        }

        List *split = StringSplit(current_line, '=');

        if (!split) {
            // fprintf(stderr, "Failed to split metadata line: %s\n", StringGetData(current_line));
            StringDestroy(current_line);
            return TXTSDL_FAILURE;
        }

        String *field = ListGet(split, 0);

        if (!field) {
            // fprintf(stderr, "Failed to get field from metadata line: %s\n", StringGetData(current_line));
            ListDestroy(split);
            ListIterate(split, (ListIterateCallback)StringDestroy);
            StringDestroy(current_line);
            return TXTSDL_FAILURE;
        }

        String *value = ListGet(split, 1);

        if (!value) {
            // fprintf(stderr, "Failed to get value from metadata line: %s\n", StringGetData(current_line));
            ListDestroy(split);
            ListIterate(split, (ListIterateCallback)StringDestroy);
            StringDestroy(current_line);
            return TXTSDL_FAILURE;
        }

        if (StringCompareCString(field, FONT_PARSE_GLYPH_WIDTH)) {
            font->glyph_width = StringToInt(value);
        } else if (StringCompareCString(field, FONT_PARSE_GLYPH_HEIGHT)) {
            font->glyph_height = StringToInt(value);
        } else if (StringCompareCString(field, FONT_PARSE_NAME)) {
            // TODO: Check if copy succeeds
            StringCopy(font->name, value);
        } else {
            // fprintf(stderr, "Invalid metadata field: %s\n", StringGetData(field));
            ListDestroy(split);
            ListIterate(split, (ListIterateCallback)StringDestroy);
            StringDestroy(current_line);
            return TXTSDL_FAILURE;
        }

        ListIterate(split, (ListIterateCallback)StringDestroy);
        ListDestroy(split);
        StringDestroy(current_line);
    }

    return TXTSDL_SUCCESS;
}

static int parseDescriptor(TxtSDLFont *font, FILE *descriptor) {
    // Each item is a string of characters
    // and represents one row in the sprite sheet
    List *character_order = ListCreate();

    String *current_line;

    // Get all of the character orders first
    while ((current_line = StringFromFileDescriptorLine(descriptor)) != NULL) {
        if (StringCompareCString(current_line, "\n")) {
            ListAdd(character_order, StringFromCString(""));
            StringDestroy(current_line);
        } else if (StringCompareCString(current_line, FONT_PARSE_METADATA_TAG)) {
            parseFontMetadata(font, descriptor);
            StringDestroy(current_line);
        } else {
            ListAdd(character_order, current_line);
        }
    }

    printf("Finished parsing descriptor\n");


    // Then create the glyph rects
    return characterOrderListToRect(font, character_order);
}

static int characterOrderListToRect(TxtSDLFont *font, List *character_order) {
    for (int y = 0; y < ListSize(character_order); y++) {
        String *current_line = ListGet(character_order, y);

        for (int x = 0; x < StringSize(current_line); x++) {
            uchar_t character = StringGetAt(current_line, x);
            SDL_Rect *rect = malloc(sizeof(SDL_Rect));

            if (!rect) {
                fprintf(stderr, "Failed to allocate memory for glyph rect\n");
                return TXTSDL_FAILURE;
            }

            rect->x = x * font->glyph_width;
            rect->y = y * font->glyph_height;
            rect->w = font->glyph_width;
            rect->h = font->glyph_height;

            HashmapInsert(font->glyph_rects, character, rect);
        }
    }

    return TXTSDL_SUCCESS;
}