#ifndef TXTSDL_FONT_H
#define TXTSDL_FONT_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../ds/charlist.h"

#define FONT_PARSE_METADATA_TAG "[METADATA]"
#define FONT_PARSE_END_TAG "[END]"
#define FONT_PARSE_GLYPH_WIDTH "glyph_width"
#define FONT_PARSE_GLYPH_HEIGHT "glyph_height"
#define FONT_PARSE_NAME "name"

typedef struct _TxtSDLFont TxtSDLFont; 
typedef struct _TxtSDLCharacterDrawInfo {
    const SDL_Rect *source;
    SDL_Texture *texture;
} TxtSDLCharacterDrawInfo;

/**
 * Creates a new TxtSDLFont from the specified image and descriptor.
 * @param name The name of the font
 * @param glyph_width The width of each glyph
 * @param glyph_height The height of each glyph
 * @param image_path The path to the image to use for the font
 * @param descriptor_path The path to the descriptor to use for the font
 * @returns A pointer to the new font
*/
TxtSDLFont *TxtSDLFont_Create(
    const char *name,
    int glyph_width,
    int glyph_height,
    const char *image_path,
    const char *descriptor_path
);

/**
 * Gets the character draw info for the specified character.
 * @param font The font to get the character draw info from
 * @param character The character to get the draw info for
 * @returns The location of the character in the font texture
*/
TxtSDLCharacterDrawInfo TxtSDLFont_GetCharacter(TxtSDLFont *font, uchar_t character);

/**
 * Checks if a font can render a specific character
*/
bool TxtSDLFont_ContainsCharacter(TxtSDLFont *font, uchar_t character);

/**
 * Frees all resources of the specified font.
 * @param font The font to destroy
*/
void TxtSDLFont_Destroy(TxtSDLFont *font);


#endif