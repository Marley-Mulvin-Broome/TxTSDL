#include <stdio.h>
#include <SDL2/SDL.h>

#include "txtsdl.h"
#include "txtsdl_char_cell.h"
#include "txtsdl_events.h"
#include "tui/tui_window.h"
#include "tui/tui_label.h"
#include "tui/tui_input.h"

#define FONT_IMG_PATH "img/no_licence/terminal.bmp"
#define FONT_PATH "img/no_licence/terminal.font"

TuiWindow *game_window;

void windowSubmitted(TxtSDLSubmitEvent *event, void *data) {
	printf("Window submitted\n");
}

void setup(TxtSDLScreen *screen) {
	game_window = TuiWindowCreate(
		"TXTSDL",
		0,
		0,
		50,
		50,
		screen
	);

	TuiLabel *label = TuiLabelCreate(5, 3, "Bad BABY");
	TuiControl *labelControl = TuiControlCreate(TUI_CONTROL_LABEL, label);
	TuiInput *input = TuiInputCreate(5, 5, 20, screen);
	TuiControl *inputControl = TuiControlCreate(TUI_CONTROL_INPUT, input);
	TuiInput *second_input = TuiInputCreate(5, 6, 20, screen);

	TuiWindowAddChild(game_window, labelControl);
	TuiWindowAddChild(game_window, inputControl);
	TuiWindowAddChild(game_window, TuiControlCreate(TUI_CONTROL_INPUT, second_input));

	TuiWindowAddSubmitEventListener(
		game_window,
		windowSubmitted
	);
}

void update(TxtSDLScreen *screen, float delta_time) {
	TuiWindowDraw(game_window);
}

int main(void) {
	TxtSDL_WindowInfo window_info = {
		"TxTSDL",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
	};

	TxtSDL_FontLoadInfo font_info = {
		.font_descriptor_path = FONT_PATH,
		.font_img_path = FONT_IMG_PATH,
		.target_height = 24,
		.target_width = 16
	};

	TxtSDL_Run(
		&window_info, 
		&font_info,
		update, 
		setup, 
		NULL
	);

	// Frees all children
	TuiWindowDestroy(game_window);

	return EXIT_SUCCESS;
}
