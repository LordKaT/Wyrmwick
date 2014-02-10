#include "include.h"

void debug_print(const char *cFmt, ...) {
#ifdef DEBUG
	char cText[512];
	va_list vArgs;
	va_start(vArgs, cFmt);
	vsnprintf(cText, 512, cFmt, vArgs);
	va_end(vArgs);
	printf("%s", cText);
#endif
	return;
}

void debug_init() {
	menu_create("Debug Menu", 0, 0, debug_menu_func);
	menu_add("Show map");
	menu_add("Sound Test");
	menu_add("Map Editor");
	menu_add("Exit");
	return;
}

void debug_input(SDL_Event *sdlEvent) {
}

/* Entered from menu_input (menu.cpp). */
void debug_menu_func(int iSelection) {
	debug_print("debug_menu_func: %i\r\n", iSelection);
	switch (iSelection) {
		case 0:
			g_iGameState = GAME_WORLD;
			debug_destroy();
			break;
		case 1:
			audio_load_music(&g_audio, "data/audio/Encounter5.mp3");
			audio_play_music(&g_audio);
			break;
		case 2:
			g_iGameState = GAME_MAP_EDITOR;
			debug_destroy();
			map_editor_init();
			break;
		case 3:
			g_bRun = false;
			debug_destroy();
			break;
		default:
			break;
	}
	return;
}

void debug_loop() {
	return;
}

void debug_destroy() {
	menu_close();
	return;
}
