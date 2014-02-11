#ifndef __KEYS_H__
#define __KEYS_H__

void input_init();
void input_save_mapping();

// maps an SDL event to a logical input event and updates global state
// TODO: this needs to be renamed. map_ are map functions in map.cpp
void map_input_event(SDL_Event e, input_event *mapped);

void input_destroy();

#endif
