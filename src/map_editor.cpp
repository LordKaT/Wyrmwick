#include "include.h"

void map_editor_init() {
	g_mapEditor.m_bDragMap = false;
	g_mapEditor.m_iActiveTile = 0;
	return;
}

void map_editor_input(SDL_Event *sdlEvent) {
	if  (sdlEvent->type == SDL_KEYDOWN) {
		if (sdlEvent->key.keysym.sym == SDLK_UP) {
			map_move(0, -1);
		}
		if (sdlEvent->key.keysym.sym == SDLK_DOWN) {
			map_move(0, 1);
		}
		if (sdlEvent->key.keysym.sym == SDLK_LEFT) {
			map_move(-1, 0);
		}
		if (sdlEvent->key.keysym.sym == SDLK_RIGHT) {
			map_move(1, 0);
		}
	}

	if (sdlEvent->type == SDL_MOUSEBUTTONDOWN) {
		debug_print("Mouse: %i (%i %i)\r\n", sdlEvent->button.button, sdlEvent->button.x, sdlEvent->button.y);
		if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) {
			g_mapEditor.m_bDragMap = true;
		}
	}

	if (sdlEvent->type == SDL_MOUSEBUTTONUP) {
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
	font_print(0, 0, "Mouse X: %i | Mouse Y: %i", g_mapEditor.m_iMouseX, g_mapEditor.m_iMouseY);
	font_print(0, 16, "Rel X: %i | Rel Y: %i", (g_mapEditor.m_iMouseX + g_map.m_rectView.x) / 32, (g_mapEditor.m_iMouseY + g_map.m_rectView.y) / 32);
	font_print(0, 32, "Dest X: %i | Dest Y: %i", g_map.m_rectDest.x, g_map.m_rectDest.y);
	font_print(0, 48, "View X: %i | View Y: %i", g_map.m_rectView.x, g_map.m_rectView.y);
	font_print(0, 64, "MRel X: %i | MRel Y: %i", g_sdlEvent.motion.xrel, g_sdlEvent.motion.yrel);
	return;
}

void map_editor_destroy() {
	return;
}
