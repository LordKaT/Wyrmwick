#include "include.h"

void map_editor_init() {
	g_mapEditor.m_bDragMap = false;
	g_mapEditor.m_iActiveTile = 2;
	return;
}

void map_editor_input(SDL_Event *sdlEvent) {
	if  (sdlEvent->type == SDL_KEYDOWN) {
		if (sdlEvent->key.keysym.sym == SDLK_LEFT) {
			g_mapEditor.m_iActiveTile--;
		}
		if (sdlEvent->key.keysym.sym == SDLK_RIGHT) {
			g_mapEditor.m_iActiveTile++;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F5) {
			map_save();
		}
	}

	if (sdlEvent->type == SDL_MOUSEBUTTONDOWN) {
		if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) {
			g_mapEditor.m_bDragMap = true;
		}
	}

	if (sdlEvent->type == SDL_MOUSEBUTTONUP) {
		if (sdlEvent->button.button == 1) {
			rect tempSrc = {g_mapEditor.m_iActiveTile * 32, 0, 32, 32};
			rect tempDst = {((g_mapEditor.m_iMouseX + g_map.m_rectView.x) / 32) * 32, ((g_mapEditor.m_iMouseY + g_map.m_rectView.y) / 32) * 32, 32, 32};
			g_map.m_map[(g_mapEditor.m_iMouseX + g_map.m_rectView.x) / 32][(g_mapEditor.m_iMouseY + g_map.m_rectView.y) / 32].m_iTileID = g_mapEditor.m_iActiveTile;
			image_draw_to(&g_map.m_imageMap, &g_map.m_imageTiles, &tempSrc, &tempDst);
			map_draw_view();
		}
		if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) {
			g_mapEditor.m_bDragMap = false;
		}
	}

	if (sdlEvent->type == SDL_MOUSEMOTION) {
		g_mapEditor.m_iMouseX = sdlEvent->motion.x;
		g_mapEditor.m_iMouseY = sdlEvent->motion.y;
		if (g_mapEditor.m_bDragMap == true) {
			map_move(sdlEvent->motion.xrel, sdlEvent->motion.yrel);
		}
	}

	return;
}

void map_editor_render() {
	map_render();

	rect tempSrc = {g_mapEditor.m_iActiveTile * 32, 0, 32, 32};
	rect tempDst = {1180, 32, 64, 64};
	image_draw(&g_map.m_imageTiles, &tempSrc, &tempDst);

	font_print(10, 10, "%s", g_map.m_cName);
	return;
}

void map_editor_destroy() {
	return;
}
