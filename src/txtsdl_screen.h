#ifndef TXTSDL_SCREEN_H
#define TXTSDL_SCREEN_H

#include "txtsdl_char_cell.h"

#include <stdbool.h>

/**
 * Represents a 2D screen of characters to be drawn to
*/
typedef struct _Screen TxtSDLScreen;

/**
 * Creates a TxtSDLScreen to be used for rendering characters
 * @param width the amount of characters along the x axis
 * @param height the amount of characters along the y axis
 * @param cell_width the width in pixels of each character cell
 * @param cell_height the height in picels of each character cell
 * @param charset_img the path to the character set images
 * @returns a pointer to a TxtSDLScreen
*/
TxtSDLScreen *TxtSDLScreen_Create(
    int width, 
    int height, 
    int cell_width,
    int cell_height,
    const char *charset_img,
    const char *charset_descriptor
);

/**
 * Writes a single character to the screen
 * @param screen pointer to a screen
 * @param x position on the x axis to print
 * @param y position on the y axis to print
 * @param cell_value the data specificying what should be displayed in that cell
 * @remarks Is the building block to all printing functions in TxtSDL
*/
void TxtSDLScreen_WriteChar(TxtSDLScreen *screen, int x, int y, const TxtSDLCharCell *cell_value);

/**
 * Writes a string of characters to the screen
 * @param screen pointer to a screen
 * @param x position on the x axis to start printing
 * @param y position on the y axis to start printing
 * @param text text to print
 * @param foreground foreground colour of the text
 * @param background background colour of the text
 * @param overflow whether or not the string should go to the next line when hitting the end. If no, it will stop printing when if it reaches the end of the screen
 * @returns TXTSDL_SUCCESS if the string was printed successfully, TXTSDL_FAILURE otherwise
*/
int TxtSDLScreen_WriteString(
    TxtSDLScreen *screen, 
    int x, 
    int y, 
    const char *text,
    const TxtSDLColour *foreground,
    const TxtSDLColour *background,
    bool overflow
);

/**
 * Clears the screen buffer of all characters
 * @param screen pointer to a screen
*/
void TxtSDLScreen_Clear(TxtSDLScreen *screen);

/**
 * Renders the current contents of the buffer to the screen, updating it for the user to see
 * @param screen pointer to a screen
*/
void TxtSDLScreen_DrawBuffer(TxtSDLScreen *screen);

/**
 * Checks whether a point is within the screen bounds
 * @param screen pointer to a screen
 * @param x position on the x axis
 * @param y position on the y axis
 * @returns true if the point is within the screen bounds, false otherwise
*/
bool TxtSDLScreen_PointInBounds(const TxtSDLScreen *screen, int x, int y);

/**
 * Checks whether a Y cooridinate is within the screen bounds
 * @param screen pointer to a screen
 * @param y position on the y axis
 * @returns true if the Y cooridinate is within the screen bounds, false otherwise
*/
bool TxtSDLScreen_YInBound(const TxtSDLScreen *screen, int y);

/**
 * Checks whether an X cooridinate is within the screen bounds
 * @param screen pointer to a screen
 * @param x position on the x axis
 * @returns true if the X cooridinate is within the screen bounds, false otherwise
*/
bool TxtSDLScreen_XInBound(const TxtSDLScreen *screen, int x);

/**
 * Destroys a screen
 * @param screen the screen to destroy
*/
void TxtSDLScreen_Destroy(TxtSDLScreen *screen);

#endif