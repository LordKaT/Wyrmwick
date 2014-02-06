#include "include.h"

void sdl2_init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL_Init: %s\r\n", SDL_GetError());
		return;
	}
	g_sdlWindow = SDL_CreateWindow(WYRMWICK_VERSION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
	if (g_sdlWindow == nullptr) {
		printf("SDL_CreateWindow: %s\r\n", SDL_GetError());
		return;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	g_sdlRenderer = SDL_CreateRenderer(g_sdlWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(g_sdlRenderer, 0x00, 0x00, 0x00, 0xff);
	SDL_UpdateWindowSurface(g_sdlWindow);
	return;
}

void sdl2_destroy() {
	SDL_DestroyRenderer(g_sdlRenderer);
	SDL_DestroyWindow(g_sdlWindow);
	SDL_Quit();
	return;
}
