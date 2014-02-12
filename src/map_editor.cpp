#include "include.h"

void map_editor_init() {
	g_mapEditor.m_bDragMap = false;
	g_mapEditor.m_iActiveTile = 2;
	g_mapEditor.m_iMapEditorState = MAPEDITOR_EDIT;
	g_mapEditor.m_cMapWalk = WALK_NONE;
	g_mapEditor.m_bGrid = false;
	return;
}

/* TODO: comment this more. Sorry --lk */
void map_editor_input(SDL_Event *sdlEvent) {
	// We may as well always check for this, because why not?
	if (sdlEvent->type == SDL_MOUSEMOTION) {
		g_mapEditor.m_iMouseX = sdlEvent->motion.x;
		g_mapEditor.m_iMouseY = sdlEvent->motion.y;
	}

	/* Generic global keydown events. */
	if  (sdlEvent->type == SDL_KEYDOWN) {
		if (sdlEvent->key.keysym.sym == SDLK_ESCAPE) {
			SDL_StopTextInput();
			map_draw_view();
			if (g_mapEditor.m_bGrid == true) {
				map_draw_grid_view();
			}
			g_mapEditor.m_iMapEditorState = MAPEDITOR_EDIT;
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F1) {
			SDL_StartTextInput();
			g_mapEditor.m_iMapEditorState = MAPEDITOR_NAME;
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F2) {
			// Choose new tile from sprite sheet
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F3) {
			// Choose new sprite sheet
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F4) {
			// Change to walk edit mode.
			if (g_mapEditor.m_iMapEditorState == MAPEDITOR_WALK) {
				g_mapEditor.m_iMapEditorState = MAPEDITOR_EDIT;
				map_draw();
				if (g_mapEditor.m_bGrid == true) {
					map_draw_grid_view();
				}
			}
			else {
				g_mapEditor.m_iMapEditorState = MAPEDITOR_WALK;
				map_editor_draw_walk();
			}
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F5) {
			map_save();
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F6) {
			// Turn grid on/off.
			if (g_mapEditor.m_bGrid == true) { // turn off grid
				g_mapEditor.m_bGrid = false;
				map_draw();
			}
			else {
				g_mapEditor.m_bGrid = true;
				map_draw_grid();
			}
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F7) {
			// Change LUA script associated with this map.
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F9) {
			SDL_StartTextInput();
			g_mapEditor.m_iMapEditorState = MAPEDITOR_LOAD;
			return;
		}
	}

	switch (g_mapEditor.m_iMapEditorState) {
		case MAPEDITOR_EDIT:
			if  (sdlEvent->type == SDL_KEYDOWN) {
				// TODO: bounds checking.
				if (sdlEvent->key.keysym.sym == SDLK_LEFT) {
					g_mapEditor.m_iActiveTile--;
				}
				if (sdlEvent->key.keysym.sym == SDLK_RIGHT) {
					g_mapEditor.m_iActiveTile++;
				}
			}

			if (sdlEvent->type == SDL_MOUSEBUTTONDOWN) { // Drag map on right click & hold
				if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) {
					g_mapEditor.m_bDragMap = true;
				}
			}

			if (sdlEvent->type == SDL_MOUSEBUTTONUP) {
				if (sdlEvent->button.button == 1) { // place tile
					rect tempSrc = {g_mapEditor.m_iActiveTile * 32, 0, 32, 32};
					rect tempDst = {((g_mapEditor.m_iMouseX + g_map.m_rectView.x) / 32) * 32, ((g_mapEditor.m_iMouseY + g_map.m_rectView.y) / 32) * 32, 32, 32};
					g_map.m_map[(g_mapEditor.m_iMouseX + g_map.m_rectView.x) / 32][(g_mapEditor.m_iMouseY + g_map.m_rectView.y) / 32].m_iTileID = g_mapEditor.m_iActiveTile;
					image_draw_to(g_map.m_imageMap, g_map.m_imageTiles, &tempSrc, &tempDst);
					map_draw_view(); // Only redraw what's currently visible (unless the map maker is psychic the current view is what changed)
					if (g_mapEditor.m_bGrid == true) {
						map_draw_grid_view();
					}
				}
				if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) { // stop dragging map
					g_mapEditor.m_bDragMap = false;
				}
			}

			if (sdlEvent->type == SDL_MOUSEMOTION) {
				if (g_mapEditor.m_bDragMap == true) {
					map_move(sdlEvent->motion.xrel, sdlEvent->motion.yrel);
				}
			}
			break;

		/* Rename map. */
		case MAPEDITOR_NAME:
			if (util_textInput(sdlEvent, &g_map.m_cName) == 1) {
				g_mapEditor.m_iMapEditorState = MAPEDITOR_EDIT;
			}
			break;

		/* Load map. */
		case MAPEDITOR_LOAD:
			if (util_textInput(sdlEvent, &g_map.m_cName) == 1) {
				map_load(g_map.m_cName);
				map_draw();
				g_mapEditor.m_iMapEditorState = MAPEDITOR_EDIT;
			}
			break;

		case MAPEDITOR_TILE:
			// Draw tilesheet
			// Input based on tile sheet, not map.
			// Choose tile on click
			// g_iMapEditorState = MAPEDITOR_EDIT
			break;

		/* collision editor. */
		case MAPEDITOR_WALK:
			if  (sdlEvent->type == SDL_KEYDOWN) {
				if (sdlEvent->key.keysym.sym == SDLK_LEFT) {
					if (g_mapEditor.m_cMapWalk > 0) {
						g_mapEditor.m_cMapWalk--;
					}
				}
				if (sdlEvent->key.keysym.sym == SDLK_RIGHT) {
					if (g_mapEditor.m_cMapWalk < WALK_MAX) {
						g_mapEditor.m_cMapWalk++;
					}
				}
			}

			if (sdlEvent->type == SDL_MOUSEBUTTONDOWN) { // Drag map on right click & hold
				if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) {
					g_mapEditor.m_bDragMap = true;
				}
			}
			if (sdlEvent->type == SDL_MOUSEBUTTONUP) {
				if (sdlEvent->button.button == 1) { // place walk
					g_map.m_map[(g_mapEditor.m_iMouseX + g_map.m_rectView.x) / 32][(g_mapEditor.m_iMouseY + g_map.m_rectView.y) / 32].m_cWalk = g_mapEditor.m_cMapWalk;
					map_draw_view(); // Only redraw what's currently visible
					map_editor_draw_walk_view();
					if (g_mapEditor.m_bGrid == true) {
						map_draw_grid_view();
					}
				}
				if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) { // stop dragging map
					g_mapEditor.m_bDragMap = false;
				}
			}

			if (sdlEvent->type == SDL_MOUSEMOTION) {
				if (g_mapEditor.m_bDragMap == true) {
					map_move(sdlEvent->motion.xrel, sdlEvent->motion.yrel);
				}
			}
			break;
		default:
			break;
	}

	return;
}

void map_editor_draw_walk() {
	rect tempRect;
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int y = 0; y < MAP_SIZE; y++) {
			if (g_map.m_map[x][y].m_cWalk > 0) {
				tempRect.x = x * 32;
				tempRect.y = y * 32;
				tempRect.w = 32;
				tempRect.h = 32;
				image_draw_fill_rect_to(g_map.m_imageMap, &tempRect, 255, 0, 255);
			}
		}
	}
	return;
}

void map_editor_draw_walk_view() {
	rect tempRect;
	for (int x = (g_map.m_rectView.x / 32); x < ((g_map.m_rectView.x + g_map.m_rectView.w) / 32); x++) {
		for (int y = (g_map.m_rectView.y / 32); y < ((g_map.m_rectView.x + g_map.m_rectView.w) / 32); y++) {
			if (g_map.m_map[x][y].m_cWalk > 0) {
				tempRect.x = x * 32;
				tempRect.y = y * 32;
				tempRect.w = 32;
				tempRect.h = 32;
				image_draw_fill_rect_to(g_map.m_imageMap, &tempRect, 255, 0, 255);
			}
		}
	}
	return;
}

void map_editor_render() {
	map_render();

	rect tempSrc = {g_mapEditor.m_iActiveTile * 32, 0, 32, 32};
	rect tempDst = {1180, 32, 64, 64};

	switch (g_mapEditor.m_iMapEditorState) {
		case MAPEDITOR_NONE:
			font_print(10, 690, "How the fuck are you even here?");
			break;
		case MAPEDITOR_EDIT:
			image_draw(g_map.m_imageTiles, &tempSrc, &tempDst);
			font_print(10, 690, "Edit Mode");
			break;
		case MAPEDITOR_NAME:
			font_print(10, 690, "Rename Map");
			break;
		case MAPEDITOR_SAVE:
			font_print(10, 690, "Saving Map");
			break;
		case MAPEDITOR_LOAD:
			font_print(10, 690, "Loading Map");
			break;
		case MAPEDITOR_TILE:
			font_print(10, 690, "Tile Select");
			break;
		case MAPEDITOR_SHEET:
			font_print(10, 690, "Sheet Select");
			break;
		case MAPEDITOR_WALK:
			image_draw(g_map.m_imageMap, &g_map.m_rectView, &g_map.m_rectDest);
			font_print(10, 690, "Walk Edit");
			switch (g_mapEditor.m_cMapWalk) {
				case WALK_NONE:
					font_print(1180, 32, "NONE");
					break;
				case WALK_WALK:
					font_print(1180, 32, "WALK");
					break;
				case WALK_RUN:
					font_print(1180, 32, "RUN");
					break;
				case WALK_SWIM:
					font_print(1180, 32, "SWIM");
					break;
				case WALK_CLIMB:
					font_print(1180, 32, "CLIMB");
					break;
				case WALK_FLY:
					font_print(1180, 32, "FLY");
					break;
				default:
					font_print(1180, 32, "BROKEN");
					break;
			}
			break;
		default:
			break;
	}

	font_print(10, 10, "%s", g_map.m_cName);
	
	return;
}

void map_editor_destroy() {
	return;
}
