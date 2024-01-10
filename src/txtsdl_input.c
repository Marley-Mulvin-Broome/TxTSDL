#include "txtsdl_input.h"

#include <stdbool.h>
#include <ctype.h>
#include <SDL2/SDL.h>

#include "txtsdl_constants.h"

bool TxtSDL_CharacterPrintable(int character) {
    return character >= 32 && character <= 126;
}

int TxtSDL_KeyPressToCharacter(int character, bool shift) {
    if (!TxtSDL_CharacterPrintable(character)) {
        return TXTSDL_FAILURE;
    }

    if (!shift) {
        return character;
    }

    return TxtSDL_GetShiftModifiedCharacter(character);
}

int TxtSDL_GetShiftModifiedCharacter(int character) {
    if (!TxtSDL_CharacterPrintable(character)) {
        return TXTSDL_FAILURE;
    }

    switch (character) {
        case SDLK_1:
            return '!';
        case SDLK_2:
            return '@';
        case SDLK_3:
            return '#';
        case SDLK_4:
            return '$';
        case SDLK_5:
            return '%';
        case SDLK_6:
            return '^';
        case SDLK_7:
            return '&';
        case SDLK_8:
            return '*';
        case SDLK_9:
            return '(';
        case SDLK_0:
            return ')';
        case SDLK_MINUS:
            return '_';
        case SDLK_EQUALS:
            return '+';
        case SDLK_LEFTBRACKET:
            return '{';
        case SDLK_RIGHTBRACKET:
            return '}';
        case SDLK_BACKSLASH:
            return '|';
        case SDLK_SEMICOLON:
            return ':';
        case SDLK_QUOTE:
            return '"';
        case SDLK_COMMA:
            return '<';
        case SDLK_PERIOD:
            return '>';
        case SDLK_SLASH:
            return '?';
        default:
            return toupper(character);
    }
}