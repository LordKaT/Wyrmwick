#include "include.h"

#define SCREEN_SDL2

void screen_init() {
	debug_print("Loading Screen ...\r\n");
#ifdef SCREEN_SDL2
	debug_print("	screen: SDL2\r\n");
	sdl2_init();
#endif
	return;
}

void screen_clear() {
#ifdef SCREEN_SDL2
	SDL_RenderClear(g_sdlRenderer);
#endif
	return;
}

void screen_present() {
#ifdef SCREEN_SDL2
	SDL_RenderPresent(g_sdlRenderer);
#endif
	return;
}

void screen_destroy() {
#ifdef SCREEN_SDL2
	sdl2_destroy();
#endif
	return;
}
