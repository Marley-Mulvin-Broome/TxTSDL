#include <stdio.h>
#include <SDL2/SDL.h>

#include "txtsdl.h"
#include "txtsdl_char_cell.h"
#include "txtsdl_events.h"
#include "tui/tui_window.h"
#include "tui/tui_label.h"

TuiWindow *game_window;
TuiControl *labelControl;

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

	TuiWindowAddChild(game_window, labelControl);
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

	TxtSDL_Run(&window_info, update, setup);

	TuiControlDestroy(labelControl);
	TuiWindowDestroy(game_window);

	return 0;
}
