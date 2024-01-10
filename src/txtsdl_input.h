#ifndef TXTSDL_INPUT_H
#define TXTSDL_INPUT_H

#include <stdbool.h>

bool TxtSDL_CharacterPrintable(int character);
int TxtSDL_KeyPressToCharacter(int character, bool shift);
int TxtSDL_GetShiftModifiedCharacter(int character);

#endif