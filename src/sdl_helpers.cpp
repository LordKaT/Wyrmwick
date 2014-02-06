#include "include.h"

void sdl_init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initalize SDL.\r\n");
		return;
	}
	g_sdlWindow = SDL_CreateWindow("Wyrmwick", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
	if (g_sdlWindow == nullptr) {
		printf("Could not create SDL window.\r\n");
		return;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	g_sdlRenderer = SDL_CreateRenderer(g_sdlWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(g_sdlRenderer, 0x00, 0x00, 0x00, 0xff);
	SDL_UpdateWindowSurface(g_sdlWindow);
	return;
}

void sdl_destroy() {
	SDL_DestroyRenderer(g_sdlRenderer);
	SDL_DestroyWindow(g_sdlWindow);
	SDL_Quit();
	return;
}
