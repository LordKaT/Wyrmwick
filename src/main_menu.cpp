#include "include.h"

static void _event(state_stack* stack, SDL_Event *sdlEvent);
static void _draw(state_stack* stack);
static void _destroy(state_stack* stack);

text_entry *text;

void main_menu_push(state_stack* stack) {
	state_desc mm = {
		GAME_MENU, nullptr,
		&main_menu_init,
		nullptr, nullptr,
		&_event,
		nullptr,
		&_draw,
		&_destroy,
		nullptr, false,
	};
	table_append(stack, &mm);
}
	
void main_menu_init(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	
	text = text_entry_init(200, 5);
	SDL_StartTextInput();
	
	menu *pMenu = menu_init(5, 5);
	menu_add_entry(pMenu, "Paly Gaem");
	menu_add_entry(pMenu, "Sound Test");
	menu_add_entry(pMenu, "Map Editor");
	menu_add_entry(pMenu, "Settings");
	menu_add_entry(pMenu, "Exit");
	menu_auto_resize(pMenu);
	top->m_pData = pMenu;
}

void _event(state_stack* stack, SDL_Event *sdlEvent) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	menu *pMenu = (menu*) top->m_pData;
	
	text_entry_input(text, sdlEvent);
	
	if (sdlEvent->type == SDL_QUIT) {
		top->m_isDead = true;
		return;
	}
	
	if (sdlEvent->type == SDL_KEYUP && sdlEvent->key.keysym.sym == SDLK_ESCAPE) {
		top->m_isDead = true;
		return;
	}
	
	int choice = menu_input(pMenu, sdlEvent);
	if (choice == -1) { return; }
	
	switch (choice) {
	case 0:
		break;
	case 1:
		audio_load_music(&g_audio, "data/audio/Encounter5.mp3");
		audio_play_music(&g_audio);
		break;
	case 2:
		top->m_fnPushChild = &map_editor_push;
		break;
	case 3:
		top->m_fnPushChild = &settings_menu_push;
		break;
	case 4:
		top->m_isDead = true;
		break;
	}
}

void _draw(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	text_entry_render(text);
	menu *pMenu = (menu*) top->m_pData;
	menu_render(pMenu);
}

void _destroy(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	menu *pMenu = (menu*) top->m_pData;
	menu_destroy(pMenu);
}

