#include "include.h"

int main(int iArgC, char * cArgV[]) {
	debug_print("Wyrmwick: %s\r\n", WYRMWICK_VERSION);
	script_init();
	screen_init();
	audio_init();
	font_init();
	menu_init();
	map_init();
	input_init();
	debug_print("Init finished!\r\n");

	g_bRun = true;
	g_iGameState = GAME_START;

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
				switch (g_iGameState) {
					case GAME_WORLD:
						/* testing map scrolling. */
						if  (g_sdlEvent.type == SDL_KEYDOWN) {
							if (g_sdlEvent.key.keysym.sym == SDLK_UP) {
								map_move(0, -1);
							}
							if (g_sdlEvent.key.keysym.sym == SDLK_DOWN) {
								map_move(0, 1);
							}
							if (g_sdlEvent.key.keysym.sym == SDLK_LEFT) {
								map_move(-1, 0);
							}
							if (g_sdlEvent.key.keysym.sym == SDLK_RIGHT) {
								map_move(1, 0);
							}
							if (g_sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
								g_iGameState = GAME_DEBUG;
							}
						}
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
			case GAME_DEBUG:
				debug_loop();
				break;
			case GAME_MENU:
				break;
			case GAME_WORLD:
				map_render();
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

	return 0;
}
