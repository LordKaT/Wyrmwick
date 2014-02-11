#include "include.h"

void map_editor_init() {
	g_mapEditor.m_bDragMap = false;
	g_mapEditor.m_iActiveTile = 2;
	return;
}

int g_iMapEditorState = MAPEDITOR_EDIT;

void map_editor_input(SDL_Event *sdlEvent) {
	// We may as well always check for this, because why not?
	if (sdlEvent->type == SDL_MOUSEMOTION) {
		g_mapEditor.m_iMouseX = sdlEvent->motion.x;
		g_mapEditor.m_iMouseY = sdlEvent->motion.y;
	}
	switch (g_iMapEditorState) {
		case MAPEDITOR_EDIT:
			if  (sdlEvent->type == SDL_KEYDOWN) {
				if (sdlEvent->key.keysym.sym == SDLK_LEFT) {
					g_mapEditor.m_iActiveTile--;
				}
				if (sdlEvent->key.keysym.sym == SDLK_RIGHT) {
					g_mapEditor.m_iActiveTile++;
				}
				if (sdlEvent->key.keysym.sym == SDLK_F1) {
					SDL_StartTextInput();
					g_iMapEditorState = MAPEDITOR_NAME;
				}
				if (sdlEvent->key.keysym.sym == SDLK_F2) {
					// Choose new tile from sprite sheet
				}
				if (sdlEvent->key.keysym.sym == SDLK_F3) {
					// Choose new sprite sheet
				}
				if (sdlEvent->key.keysym.sym == SDLK_F5) {
					map_save();
				}
				if (sdlEvent->key.keysym.sym == SDLK_F9) {
					SDL_StartTextInput();
					g_iMapEditorState = MAPEDITOR_LOAD;
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
					image_draw_to(&g_map.m_imageMap, &g_map.m_imageTiles, &tempSrc, &tempDst);
					map_draw_view(); // Only redraw what's currently visible (unless the map maker is psychic the current view is what changed.
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
			if (sdlEvent->type == SDL_KEYDOWN) {
				if (sdlEvent->key.keysym.sym == SDLK_BACKSPACE) { // Handle backspace
					for (unsigned int i = 0; i < strlen(g_map.m_cName)+1; i++) {
						if (g_map.m_cName[i] == '\0' && i > 0) {
							g_map.m_cName[i - 1] = '\0';
						}
					}
				}
				if (sdlEvent->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) { // Paste from clipboad (SDL_GetClipboardText())
					char *cTemp;
					char *cClipboardText = SDL_GetClipboardText();
					if (cClipboardText == nullptr) {
						break;
					}
					cTemp = (char *)malloc(sizeof(char) * strlen(g_map.m_cName) + 1);
					memset(cTemp, 0, sizeof(char) * strlen(g_map.m_cName) + 1);
					strcpy(cTemp, g_map.m_cName);
					if (g_map.m_cName != nullptr) {
						free(g_map.m_cName);
						g_map.m_cName = nullptr;
					}
					g_map.m_cName = (char *)malloc(sizeof(char) * strlen(cTemp) + strlen(cClipboardText) + 2);
					strcpy(g_map.m_cName, cTemp);
					strcat(g_map.m_cName, cClipboardText);
					break;
				}
				if (sdlEvent->key.keysym.sym == SDLK_RETURN) {
					g_iMapEditorState = MAPEDITOR_EDIT;
					SDL_StopTextInput();
					break;
				}
			}
			if (sdlEvent->type == SDL_TEXTINPUT) {
				if (!((sdlEvent->text.text[0] == 'v' || sdlEvent->text.text[0] == 'V' ) && SDL_GetModState() & KMOD_CTRL)) { // Ignore CTRL+v
					char *cTemp;
					cTemp = (char *)malloc(sizeof(char) * strlen(g_map.m_cName) + 1);
					memset(cTemp, 0, sizeof(char) * strlen(g_map.m_cName) + 1);
					strcpy(cTemp, g_map.m_cName);
					if (g_map.m_cName != nullptr) {
						free(g_map.m_cName);
						g_map.m_cName = nullptr;
					}
					g_map.m_cName = (char *)malloc(sizeof(char) * strlen(cTemp) + strlen(sdlEvent->text.text) + 2);
					strcpy(g_map.m_cName, cTemp);
					strcat(g_map.m_cName, sdlEvent->text.text);
				}
			}
			break;

		/* Map Loading. */
		case MAPEDITOR_LOAD:
			if (sdlEvent->type == SDL_KEYDOWN) {
				if (sdlEvent->key.keysym.sym == SDLK_BACKSPACE) { // Handle backspace
					for (unsigned int i = 0; i < strlen(g_map.m_cName)+1; i++) {
						if (g_map.m_cName[i] == '\0' && i > 0) {
							g_map.m_cName[i - 1] = '\0';
						}
					}
				}
				if (sdlEvent->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) { // Paste from clipboad (SDL_GetClipboardText())
					char *cTemp;
					char *cClipboardText = SDL_GetClipboardText();
					if (cClipboardText == nullptr) {
						break;
					}
					cTemp = (char *)malloc(sizeof(char) * strlen(g_map.m_cName) + 1);
					memset(cTemp, 0, sizeof(char) * strlen(g_map.m_cName) + 1);
					strcpy(cTemp, g_map.m_cName);
					if (g_map.m_cName != nullptr) {
						free(g_map.m_cName);
						g_map.m_cName = nullptr;
					}
					g_map.m_cName = (char *)malloc(sizeof(char) * strlen(cTemp) + strlen(cClipboardText) + 2);
					strcpy(g_map.m_cName, cTemp);
					strcat(g_map.m_cName, cClipboardText);
					break;
				}
				if (sdlEvent->key.keysym.sym == SDLK_RETURN) {
					map_load(g_map.m_cName);
					map_draw();
					g_iMapEditorState = MAPEDITOR_EDIT;
					SDL_StopTextInput();
					break;
				}
			}
			if (sdlEvent->type == SDL_TEXTINPUT) {
				if (!((sdlEvent->text.text[0] == 'v' || sdlEvent->text.text[0] == 'V' ) && SDL_GetModState() & KMOD_CTRL)) { // Ignore CTRL+v
					char *cTemp;
					cTemp = (char *)malloc(sizeof(char) * strlen(g_map.m_cName) + 1);
					memset(cTemp, 0, sizeof(char) * strlen(g_map.m_cName) + 1);
					strcpy(cTemp, g_map.m_cName);
					if (g_map.m_cName != nullptr) {
						free(g_map.m_cName);
						g_map.m_cName = nullptr;
					}
					g_map.m_cName = (char *)malloc(sizeof(char) * strlen(cTemp) + strlen(sdlEvent->text.text) + 2);
					strcpy(g_map.m_cName, cTemp);
					strcat(g_map.m_cName, sdlEvent->text.text);
				}
			}
			break;
		case MAPEDITOR_TILE:
			// Draw tilesheet
			// Input based on tile sheet, not map.
			// Choose tile on click
			// g_iMapEditorState = MAPEDITOR_EDIT
			break;
		default:
			break;
	}

	return;
}

void map_editor_render() {
	map_render();

	rect tempSrc = {g_mapEditor.m_iActiveTile * 32, 0, 32, 32};
	rect tempDst = {1180, 32, 64, 64};
	image_draw(&g_map.m_imageTiles, &tempSrc, &tempDst);

	font_print(10, 10, "%s", g_map.m_cName);

	switch (g_iMapEditorState) {
		case MAPEDITOR_NONE:
			font_print(10, 690, "How the fuck are you even here?");
			break;
		case MAPEDITOR_EDIT:
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
		default:
			break;
	}
	return;
}

void map_editor_destroy() {
	return;
}
