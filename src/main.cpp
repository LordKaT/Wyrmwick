#include "include.h"

int main(int iArgC, char * cArgV[]) {
	debug_print("Wyrmwick: %s\r\n", WYRMWICK_VERSION);
	
	settings *Settings = settings_new();
	input_config_settings(Settings, g_inmap);
	screen_config_settings(Settings, nullptr);
	settings_load(Settings, settings_file_path);
	
	script_init();
	screen_init();
	audio_init();
	font_init();
	menu_init();
	map_init();
	input_init();
	debug_print("Init finished!\r\n");
	
	// This is for debugging only. I don't think we want to obliterate the config every time,
	// especially if there were parsing errors.
	settings_save(Settings, settings_file_path);

	g_bRun = true;
	map_editor_init();
	g_iGameState = GAME_MAP_EDITOR;

	while (g_bRun == true) {
		while (SDL_PollEvent(&g_sdlEvent)) {
			if (g_sdlEvent.type == SDL_QUIT) {
				g_bRun = false;
			}
			if (menu_is_open()) {
				menu_input(&g_sdlEvent);
			}
			else {
				/* input. */
				if  (g_sdlEvent.type == SDL_KEYDOWN) {
					/* no matter what, go to the debug menu. */
					if (g_sdlEvent.key.keysym.sym == SDLK_F11) {
						g_iGameState = GAME_DEBUG;
						debug_init();
					}
				}
				switch (g_iGameState) {
					case GAME_WORLD:
						if  (g_sdlEvent.type == SDL_KEYDOWN) {
							if (g_sdlEvent.key.keysym.sym == SDLK_F9) {
								map_editor_init();
								g_iGameState = GAME_MAP_EDITOR;
							}
						}
						break;
					case GAME_MAP_EDITOR:
						map_editor_input(&g_sdlEvent);
						break;
					default:
						break;
				}
			}
		}
		screen_clear();

		/* game logic, drawing. */
		switch (g_iGameState) {
			case GAME_START:
				break;
			case GAME_DEBUG:
				debug_loop();
				break;
			case GAME_MENU:
				break;
			case GAME_WORLD:
				map_render();
				break;
			case GAME_MAP_EDITOR:
				map_editor_render();
				break;
			default:
				break;
		}

		/*
			Rendering should take place outside the game logic.
			Pain in the ass concept, but it's a lot better than tying
			everything to one system. Makes upgrading later on easier.

			Of course, that means testing things out is a royal pain in
			the ass, since the whole system needs to be implemented before
			we're able to do anything basic.

			In other words: I'm working on it. --lk
		*/

		if (menu_is_open()) {
			menu_render();
		}
		screen_present();
	}

	menu_destroy();
	font_destroy();
	input_destroy();
	screen_destroy();
	settings_free(Settings);
	
	return 0;
}
