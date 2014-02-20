#include "include.h"

int main(int iArgC, char * cArgV[]) {
	debug_print("Wyrmwick: %s\r\n", WYRMWICK_VERSION);
	
	settings *Settings = settings_new();
	input_config_settings(Settings, g_inmap);
	screen_config_settings(Settings, nullptr);
	settings_load(Settings, settings_file_path);
	
	// Testing listing
	sys_dir *dir = sys_dir_open(".");
	const char *f;
	while ( (f = sys_dir_next(dir)) ) {
		printf("%s\r\n", f);
	}
	sys_dir_close(dir);
	
	script_init();
	screen_init();
	audio_init();
	font_init();
	map_init();
	input_init();
	debug_print("Init finished!\r\n");
	
	// This is for debugging only. I don't think we want to obliterate the config every time,
	// especially if there were parsing errors.
	settings_save(Settings, settings_file_path);

	state_stack *gameStateStack = table_new(sizeof(state_desc), 0, 0);
	main_menu_push(gameStateStack);
	main_menu_init(gameStateStack);
	
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
			currentState->m_fnPushChild(gameStateStack);
			
			currentState = (state_desc*) table_ind(gameStateStack, gameStateStack->m_len-1);
			currentState->m_fnInit(gameStateStack);
		}
	}

	font_destroy();
	input_destroy();
	screen_destroy();
	settings_free(Settings);
	
	return 0;
}
