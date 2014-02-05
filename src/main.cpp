#include "include.h"

int main(int iArgC, char * cArgV[]) {
	printf("Wyrmwick: %s\r\n", WYRMWICK_VERSION);
	printf("Loading ...\r\n");
	printf("LUA 5.2.3 ... ");
	script_init();
	printf("Loading items ...\r\n");
	script_load_items();
	printf("Loading skills ...\r\n");
	script_load_skills();
	printf("Loading quests ...\r\n");
	script_load_quests();
	printf("Loading SDL ...\r\n");
	sdl_init();
	printf("Loading Font ...\r\n");
	font_init();
	printf("Loading Menu ...\r\n");
	menu_init();
	printf( "Done!\r\n");

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
		}
		debug_loop();
		SDL_RenderClear(g_sdlRenderer);
		if (menu_is_open()) {
			menu_render();
		}
		SDL_RenderPresent(g_sdlRenderer);
	}

	menu_destroy();
	font_destroy();
	sdl_destroy();

	return 0;
}
