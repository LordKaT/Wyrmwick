#include "include.h"

int main(int iArgC, char * cArgV[]) {
	debug_print("Wyrmwick: %s\n", WYRMWICK_VERSION);
	
	settings *Settings = settings_init();
	input_config_settings(Settings, g_keybinds);
	screen_config_settings(Settings, nullptr);
	settings_load(Settings, settings_file_path);
	
	script_init();
	// useful for testing Lua scripts.
	if (iArgC >= 2 && strcmp(cArgV[1], "-q") == 0) {
		return 0;
	}
	
	screen_init();
	audio_init();
	g_font = font_init("data/images/fonts/437_16x16.png");
	map_init();
	input_init();
	debug_print("Init finished!\n");
	
	state_stack *gameStateStack = table_new(sizeof(state_desc), 0, 0);
	lua_pushlightuserdata(g_luaState, gameStateStack);
	lua_setfield(g_luaState, LUA_REGISTRYINDEX, "game/stateStack");
	main_menu_push(gameStateStack, nullptr);
	
	state_desc *currentState;
	
	SDL_Event sdlEvent;
	
	Uint32 time, lastTime;
	lastTime = SDL_GetTicks();

	// Run main loop as long as we have a state to execute.
	while(gameStateStack->m_len != 0) {
		currentState = (state_desc*) table_ind(gameStateStack, gameStateStack->m_len-1);
		
		while (SDL_PollEvent(&sdlEvent)) {
			// Emergency self-destruct on Ctrl-Shift-F4
			if (sdlEvent.type == SDL_KEYUP && sdlEvent.key.keysym.sym == SDLK_F4 &&
					(sdlEvent.key.keysym.mod & KMOD_SHIFT) != 0 &&
					(sdlEvent.key.keysym.mod & KMOD_CTRL) != 0) {
				sys_abort();
			}
			
			input_joystick_update(&sdlEvent);
			
			if (currentState->m_fnEvent != nullptr) { currentState->m_fnEvent(gameStateStack, &sdlEvent); }
		}
		
		time = SDL_GetTicks();
		if (currentState->m_fnUpdate != nullptr) { currentState->m_fnUpdate(gameStateStack, time-lastTime); }
		lastTime = time;
		
		screen_clear();
		currentState->m_fnDraw(gameStateStack);
		screen_present();
		
		if (currentState->m_isDead) {
			// Destroy and pop the current state.
			currentState->m_fnDestroy(gameStateStack);
			table_shrink(gameStateStack, 1);
			// Resume the last state, if any.
			if (gameStateStack->m_len != 0) {
				currentState = (state_desc*) table_ind(gameStateStack, gameStateStack->m_len-1);
				if (currentState->m_fnResume != nullptr) { currentState->m_fnResume(gameStateStack); }
			}
			continue;
		}
		
		if (currentState->m_fnPushChild != nullptr) {
			if (currentState->m_fnSuspend != nullptr) { currentState->m_fnSuspend(gameStateStack); }
			currentState->m_fnPushChild(gameStateStack, currentState->m_pChildData);
			currentState->m_fnPushChild = nullptr;
			currentState->m_pChildData = nullptr;
		}
	}

	font_destroy(g_font);
	input_destroy();
	screen_destroy();
	audio_destroy();
	settings_destroy(Settings);
	
	return 0;
}
