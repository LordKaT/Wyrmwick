#include "include.h"

void debug_input() {
}

void debug_menu_func(int iSelection) {
	DEBUG_PRINT(("debug_menu_func: %i\r\n", iSelection));
	if (iSelection == 1) {
		g_bRun = false;
	}
	return;
}

void debug_loop() {
	if (g_iGameState != GAME_DEBUG) {
		g_iGameState = GAME_DEBUG;
		menu_create("Debug Menu", debug_menu_func);
		menu_add("List item by ID");
		menu_add("Exit");
	}
	return;
}

