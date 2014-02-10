#ifndef __KEYS_H__
#define __KEYS_H__

void input_init();
void input_load_mapping();
void input_save_mapping();

// maps an SDL event to a logical input event and updates global state
// TODO: this needs to be renamed. map_ are map functions in map.cpp
void map_input_event(SDL_Event e, input_event *mapped);

// Grabs input, puts it inot a buffer, and returns said buffer
// after the player hits SDLK_RETURN
char *input_inputText(SDL_Event *sdlEvent, char *cBuffer);

void input_destroy();

#endif
