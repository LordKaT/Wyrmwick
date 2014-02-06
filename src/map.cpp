#include "include.h"

void map_init() {
	debug_print("Loading Map ...\r\n");
	SDL_Surface *surfTemp = SDL_LoadBMP("data/images/tiles/test_tile.bmp");
	if (surfTemp == nullptr) {
		printf("map_init(): %s\r\n", SDL_GetError());
		return;
	}
	g_sdlMapTex = SDL_CreateTextureFromSurface(g_sdlRenderer, surfTemp);
	if (g_sdlMapTex == nullptr) {
		printf("Unable to convert image to texture (why?)\r\n");
		return;
	}
	g_map.m_tex = SDL_CreateTexture(g_sdlRenderer, SDL_GetWindowPixelFormat(g_sdlWindow), SDL_TEXTUREACCESS_TARGET, 2560, 1440);
	SDL_SetTextureBlendMode(g_map.m_tex, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(g_sdlRenderer, g_map.m_tex);
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			g_map.m_rect[x][y].x = x * 32;
			g_map.m_rect[x][y].y = y * 32;
			g_map.m_rect[x][y].w = 32;
			g_map.m_rect[x][y].h = 32;
			SDL_RenderCopy(g_sdlRenderer, g_sdlMapTex, nullptr, &g_map.m_rect[x][y]);
		}
	}
	SDL_SetRenderTarget(g_sdlRenderer, nullptr);

	g_map.m_rectView.x = 0;
	g_map.m_rectView.y = 0;
	g_map.m_rectView.w = 1280;
	g_map.m_rectView.h = 720;

	g_map.m_rectDest.x = 0;
	g_map.m_rectDest.y = 0;
	g_map.m_rectDest.w = 1280;
	g_map.m_rectDest.h = 720;
	SDL_FreeSurface(surfTemp);
	return;
}

void map_move(int iX, int iY) {
	g_map.m_rectView.x += iX;
	g_map.m_rectView.y += iY;
	return;
}

void map_render() {
	SDL_RenderCopy(g_sdlRenderer, g_map.m_tex, &g_map.m_rectView, &g_map.m_rectDest);
	return;
}

void map_destroy() {
	return;
}
