#include <stdio.h>
#include <SDL2/SDL.h>

#include "txtsdl.h"
#include "txtsdl_char_cell.h"
#include "txtsdl_events.h"
#include "tui/tui_window.h"
#include "tui/tui_label.h"
#include "tui/tui_input.h"

TuiWindow *game_window;
TuiControl *labelControl;
TuiControl *inputControl;

void setup(TxtSDLScreen *screen) {
	game_window = TuiWindowCreate(
		"TXTSDL",
		0,
		0,
		50,
		50,
		screen
	);

	TuiLabel *label = TuiLabelCreate(5, 3, "HELLO WORLD");
	labelControl = TuiControlCreate(TUI_CONTROL_LABEL, label);
	TuiInput *input = TuiInputCreate(5, 5, 10, screen);
	inputControl = TuiControlCreate(TUI_CONTROL_INPUT, input);

	TuiWindowAddChild(game_window, labelControl);
	TuiWindowAddChild(game_window, inputControl);
}

void update(TxtSDLScreen *screen, float delta_time) {
	TuiWindowDraw(game_window);
}

int main(void) {
	TxtSDL_WindowInfo window_info = {
		"TxTSDL",
		0,
		0,
		800,
		600,
		SDL_WINDOW_SHOWN
	};

	TxtSDL_Run(
		&window_info, 
		"img/charset.bmp", 
		"charset/charset.font", 
		update, 
		setup, 
		NULL
	);

	// Frees all children
	TuiWindowDestroy(game_window);

	return EXIT_SUCCESS;
}
