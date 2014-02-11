#ifndef __KEYS_H__
#define __KEYS_H__

void input_config_settings(settings *st, input_control *controls);

void input_init();
void input_save_mapping();

// maps an SDL event to a logical input event and updates global state
void input_get_event(SDL_Event e, input_event *mapped);

void input_destroy();

#endif
