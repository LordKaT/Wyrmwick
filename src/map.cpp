#include "include.h"

void map_init() {
	debug_print("Loading Map ...\r\n");
	g_map.m_imageTiles = image_load("data/images/tiles/test_tile.bmp", false, 0, 0, 0);
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			g_map.m_rect[x][y].x = x * 32;
			g_map.m_rect[x][y].y = y * 32;
			g_map.m_rect[x][y].w = 32;
			g_map.m_rect[x][y].h = 32;
			image_draw_to(&g_map.m_imageMap, &g_map.m_imageTiles, nullptr, &g_map.m_rect[x][y]);
		}
	}

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

void map_load(char *cMap) {

	return;
}

void map_move(int iX, int iY) {
	g_map.m_rectView.x += iX;
	g_map.m_rectView.y += iY;
	return;
}

void map_render() {
	image_draw(&g_map.m_imageMap, &g_map.m_rectView, &g_map.m_rectDest);
	return;
}

void map_destroy() {
	return;
}
