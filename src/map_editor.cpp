#include "include.h"

rect _tile_rect(int index) {
	rect r;
	r.x = (index % 32) * 32;
	r.y = (index / 32) * 32;
	r.w = 32;
	r.h = 32;
	return r;
}

void map_editor_push(state_stack* stack) {
	state_desc editor = {
		GAME_MAP_EDITOR, nullptr,
		&map_editor_init,
		nullptr, nullptr,
		&map_editor_input,
		nullptr,
		&map_editor_render,
		&map_editor_destroy,
		nullptr, false,
	};
	table_append(stack, &editor);
}

void map_editor_init(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	map_editor *mapEditor = (map_editor*) malloc(sizeof(map_editor));
	
	map_init();

	mapEditor->m_bDragMap = false;
	mapEditor->m_iActiveTile = 2;
	mapEditor->m_iMapEditorState = MAPEDITOR_EDIT;
	mapEditor->m_cMapWalk = WALK_NONE;
	mapEditor->m_bGrid = false;
	top->m_pData = mapEditor;
	
	// TODO: Stuff the map into the map editor properly.
	return;
}

/* TODO: comment this more. Sorry --lk */
void map_editor_input(state_stack* stack, SDL_Event *sdlEvent) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	map_editor *mapEditor = (map_editor*) top->m_pData;
	
	if (sdlEvent->type == SDL_QUIT) {
		// TODO: Check for unsaved changes
		state_stack_kill(stack);
	}
	
	// We may as well always check for this, because why not?
	if (sdlEvent->type == SDL_MOUSEMOTION) {
		mapEditor->m_iMouseX = sdlEvent->motion.x;
		mapEditor->m_iMouseY = sdlEvent->motion.y;
	}

	/* Generic global keydown events. */
	if  (sdlEvent->type == SDL_KEYDOWN) {
		if (sdlEvent->key.keysym.sym == SDLK_ESCAPE) {
			SDL_StopTextInput();
			map_draw_view();
			if (mapEditor->m_bGrid == true) {
				map_draw_grid_view();
			}
			mapEditor->m_iMapEditorState = MAPEDITOR_EDIT;
			return;
		}
		if (sdlEvent->key.keysym.sym == SDLK_F1) {
			SDL_StartTextInput();
			mapEditor->m_iMapEditorState = MAPEDITOR_NAME;
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
			if (mapEditor->m_iMapEditorState == MAPEDITOR_WALK) {
				mapEditor->m_iMapEditorState = MAPEDITOR_EDIT;
				map_draw();
				if (mapEditor->m_bGrid == true) {
					map_draw_grid_view();
				}
			}
			else {
				mapEditor->m_iMapEditorState = MAPEDITOR_WALK;
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
			if (mapEditor->m_bGrid == true) { // turn off grid
				mapEditor->m_bGrid = false;
				map_draw();
			}
			else {
				mapEditor->m_bGrid = true;
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
			mapEditor->m_iMapEditorState = MAPEDITOR_LOAD;
			return;
		}
	}

	switch (mapEditor->m_iMapEditorState) {
		case MAPEDITOR_EDIT:
			if  (sdlEvent->type == SDL_KEYDOWN) {
				// TODO: bounds checking.
				if (sdlEvent->key.keysym.sym == SDLK_LEFT) {
					mapEditor->m_iActiveTile--;
				}
				if (sdlEvent->key.keysym.sym == SDLK_RIGHT) {
					mapEditor->m_iActiveTile++;
				}
			}

			if (sdlEvent->type == SDL_MOUSEBUTTONDOWN) { // Drag map on right click & hold
				if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) {
					mapEditor->m_bDragMap = true;
				}
			}

			if (sdlEvent->type == SDL_MOUSEBUTTONUP) {
				if (sdlEvent->button.button == 1) { // place tile
					rect tempSrc = _tile_rect(mapEditor->m_iActiveTile);
					rect tempDst = {((mapEditor->m_iMouseX + g_map.m_rectView.x) / 32) * 32, ((mapEditor->m_iMouseY + g_map.m_rectView.y) / 32) * 32, 32, 32};
					g_map.m_map[(mapEditor->m_iMouseX + g_map.m_rectView.x) / 32][(mapEditor->m_iMouseY + g_map.m_rectView.y) / 32].m_iTileID = mapEditor->m_iActiveTile;
					image_draw_to(g_map.m_imageMap, g_map.m_imageTiles, &tempSrc, &tempDst);
					map_draw_view(); // Only redraw what's currently visible (unless the map maker is psychic the current view is what changed)
					if (mapEditor->m_bGrid == true) {
						map_draw_grid_view();
					}
				}
				if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) { // stop dragging map
					mapEditor->m_bDragMap = false;
				}
			}

			if (sdlEvent->type == SDL_MOUSEMOTION) {
				if (mapEditor->m_bDragMap == true) {
					map_move(sdlEvent->motion.xrel, sdlEvent->motion.yrel);
				}
			}
			break;

		/* Rename map. */
		case MAPEDITOR_NAME:
			if (util_textInput(sdlEvent, &g_map.m_cName) == 1) {
				mapEditor->m_iMapEditorState = MAPEDITOR_EDIT;
			}
			break;

		/* Load map. */
		case MAPEDITOR_LOAD:
			if (util_textInput(sdlEvent, &g_map.m_cName) == 1) {
				map_load(g_map.m_cName);
				map_draw();
				mapEditor->m_iMapEditorState = MAPEDITOR_EDIT;
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
					if (mapEditor->m_cMapWalk > 0) {
						mapEditor->m_cMapWalk--;
					}
				}
				if (sdlEvent->key.keysym.sym == SDLK_RIGHT) {
					if (mapEditor->m_cMapWalk < WALK_MAX) {
						mapEditor->m_cMapWalk++;
					}
				}
			}

			if (sdlEvent->type == SDL_MOUSEBUTTONDOWN) { // Drag map on right click & hold
				if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) {
					mapEditor->m_bDragMap = true;
				}
			}
			if (sdlEvent->type == SDL_MOUSEBUTTONUP) {
				if (sdlEvent->button.button == 1) { // place walk
					g_map.m_map[(mapEditor->m_iMouseX + g_map.m_rectView.x) / 32][(mapEditor->m_iMouseY + g_map.m_rectView.y) / 32].m_cWalk = mapEditor->m_cMapWalk;
					map_draw_view(); // Only redraw what's currently visible
					map_editor_draw_walk_view();
					if (mapEditor->m_bGrid == true) {
						map_draw_grid_view();
					}
				}
				if (sdlEvent->button.button == 2 || sdlEvent->button.button == 3) { // stop dragging map
					mapEditor->m_bDragMap = false;
				}
			}

			if (sdlEvent->type == SDL_MOUSEMOTION) {
				if (mapEditor->m_bDragMap == true) {
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

void map_editor_render(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	map_editor *mapEditor = (map_editor*) top->m_pData;

	map_render();

	rect tempSrc = _tile_rect(mapEditor->m_iActiveTile);
	rect tempDst = {1180, 32, 64, 64};

	switch (mapEditor->m_iMapEditorState) {
		case MAPEDITOR_NONE:
			font_print(g_font, 10, 690, "How the fuck are you even here?");
			break;
		case MAPEDITOR_EDIT:
			image_draw(g_map.m_imageTiles, &tempSrc, &tempDst);
			font_print(g_font, 10, 690, "Edit Mode");
			break;
		case MAPEDITOR_NAME:
			font_print(g_font, 10, 690, "Rename Map");
			break;
		case MAPEDITOR_SAVE:
			font_print(g_font, 10, 690, "Saving Map");
			break;
		case MAPEDITOR_LOAD:
			font_print(g_font, 10, 690, "Loading Map");
			break;
		case MAPEDITOR_TILE:
			font_print(g_font, 10, 690, "Tile Select");
			break;
		case MAPEDITOR_SHEET:
			font_print(g_font, 10, 690, "Sheet Select");
			break;
		case MAPEDITOR_WALK:
			image_draw(g_map.m_imageMap, &g_map.m_rectView, &g_map.m_rectDest);
			font_print(g_font, 10, 690, "Walk Edit");
			switch (mapEditor->m_cMapWalk) {
				case WALK_NONE:
					font_print(g_font, 1180, 32, "NONE");
					break;
				case WALK_WALK:
					font_print(g_font, 1180, 32, "WALK");
					break;
				case WALK_RUN:
					font_print(g_font, 1180, 32, "RUN");
					break;
				case WALK_SWIM:
					font_print(g_font, 1180, 32, "SWIM");
					break;
				case WALK_CLIMB:
					font_print(g_font, 1180, 32, "CLIMB");
					break;
				case WALK_FLY:
					font_print(g_font, 1180, 32, "FLY");
					break;
				default:
					font_print(g_font, 1180, 32, "BROKEN");
					break;
			}
			break;
		default:
			break;
	}

	font_print(g_font, 10, 10, "%s", g_map.m_cName);
	
	return;
}

void map_editor_destroy(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	map_editor *mapEditor = (map_editor*) top->m_pData;
	free(mapEditor);
	return;
}
