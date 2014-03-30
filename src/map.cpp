#include "include.h"

void map_init() {
	debug_print("Loading Map ...\n");
	// FIXME: This is never freed.
	g_map.m_cName = strdup("Debug Map");
	g_map.m_imageTiles = image_load("data/images/tiles/Goo-13022014_TileSet_Test.bmp", false, 0, 0, 0);
	g_map.m_imageMap = image_create_texture(MAP_TEXTURE_SIZE, MAP_TEXTURE_SIZE);

	for (int x = 0; x < MAP_SIZE; x++) {
		for (int y = 0; y < MAP_SIZE; y++) {
			g_map.m_map[x][y].m_iTileID = 0;
		}
	}

	map_load("Debug Map");
	map_draw();

	g_map.m_rectView.x = 0;
	g_map.m_rectView.y = 0;
	g_map.m_rectView.w = 1280;
	g_map.m_rectView.h = 720;

	g_map.m_rectDest.x = 0;
	g_map.m_rectDest.y = 0;
	g_map.m_rectDest.w = 1280;
	g_map.m_rectDest.h = 720;
	return;
}

void map_draw() {
	rect tempRect;
	rect tempTile;
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int y = 0; y < MAP_SIZE; y++) {
			tempRect.x = x * 32;
			tempRect.y = y * 32;
			tempRect.w = 32;
			tempRect.h = 32;

			tempTile.x = g_map.m_map[x][y].m_iTileID * 32;
			tempTile.y = 0;
			tempTile.w = 32;
			tempTile.h = 32;
			image_draw_to(g_map.m_imageMap, g_map.m_imageTiles, &tempTile, &tempRect);
		}
	}
	return;
}

/* Only redraw the map in the visible view. */
void map_draw_view() {
	rect tempRect;
	rect tempTile;
	for (int x = (g_map.m_rectView.x / 32); x < ((g_map.m_rectView.x + g_map.m_rectView.w) / 32); x++) {
		for (int y = (g_map.m_rectView.y / 32); y < ((g_map.m_rectView.x + g_map.m_rectView.w) / 32); y++) {
			tempRect.x = x * 32;
			tempRect.y = y * 32;
			tempRect.w = 32;
			tempRect.h = 32;

			tempTile.x = g_map.m_map[x][y].m_iTileID * 32;
			tempTile.y = 0;
			tempTile.w = 32;
			tempTile.h = 32;

			image_draw_to(g_map.m_imageMap, g_map.m_imageTiles, &tempTile, &tempRect);
		}
	}
	return;
}

void map_draw_grid() {
	rect tempRect;
	// Not portable code, change this later
	SDL_SetRenderTarget(g_sdlRenderer, g_map.m_imageMap);
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int y = 0; y < MAP_SIZE; y++) {
			tempRect.x = x * 32;
			tempRect.y = y * 32;
			tempRect.w = 32;
			tempRect.h = 32;
			SDL_RenderDrawRect(g_sdlRenderer, &tempRect);
		}
	}
	SDL_SetRenderTarget(g_sdlRenderer, nullptr);
	return;
}

void map_draw_grid_view() {
	rect tempRect;
	// Not portable code, change this later
	SDL_SetRenderTarget(g_sdlRenderer, g_map.m_imageMap);
	for (int x = (g_map.m_rectView.x / 32); x < ((g_map.m_rectView.x + g_map.m_rectView.w) / 32); x++) {
		for (int y = (g_map.m_rectView.y / 32); y < ((g_map.m_rectView.x + g_map.m_rectView.w) / 32); y++) {
			tempRect.x = x * 32;
			tempRect.y = y * 32;
			tempRect.w = 32;
			tempRect.h = 32;
			SDL_RenderDrawRect(g_sdlRenderer, &tempRect);
		}
	}
	SDL_SetRenderTarget(g_sdlRenderer, nullptr);
}

void map_load(const char *cMap) {
	char *cFile;
	cFile = (char *)malloc(sizeof(char) * (strlen(cMap) + strlen("data/maps/.map") + 2));
	strcpy(cFile, "data/maps/");
	strcat(cFile, cMap);
	strcat(cFile, ".map");
	FILE *file = fopen(cFile, "rb");
	if (file == nullptr) {
		debug_print("map_load(): could not open file: %s\n", cFile);
		return;
	}
	fread(g_map.m_map, sizeof(struct mapData), MAP_SIZE*MAP_SIZE, file);
	fclose(file);
	free(cFile);
	return;
}

void map_save() {
	char *cFile;
	cFile = (char *)malloc(sizeof(char) * (strlen(g_map.m_cName) + strlen("data/maps/.map") + 2));
	strcpy(cFile, "data/maps/");
	strcat(cFile, g_map.m_cName);
	strcat(cFile, ".map");
	FILE *file = fopen(cFile, "wb+");
	if (file == nullptr) {
		debug_print("map_save(): could not open file: %s\n", cFile);
		return;
	}
	fwrite(g_map.m_map, sizeof(struct mapData), MAP_SIZE*MAP_SIZE, file);
	fclose(file);
	free(cFile);
	return;
}

void map_move(int iX, int iY) {
	g_map.m_rectView.x -= iX;
	g_map.m_rectView.y -= iY;

	if (g_map.m_rectView.x < 0) {
		g_map.m_rectView.x = 0;
	}
	if (g_map.m_rectView.x > MAP_TEXTURE_SIZE - SCREEN_WIDTH) {
		g_map.m_rectView.x = MAP_TEXTURE_SIZE - SCREEN_WIDTH;
	}
	if (g_map.m_rectView.y < 0) {
		g_map.m_rectView.y = 0;
	}
	if (g_map.m_rectView.y > MAP_TEXTURE_SIZE - SCREEN_HEIGHT) {
		g_map.m_rectView.y = MAP_TEXTURE_SIZE - SCREEN_HEIGHT;
	}
	return;
}

void map_render() {
	image_draw(g_map.m_imageMap, &g_map.m_rectView, &g_map.m_rectDest);
	return;
}

void map_destroy() {
	return;
}
