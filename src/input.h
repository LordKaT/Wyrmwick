#ifndef __KEYS_H__
#define __KEYS_H__

void input_config_settings(settings *st, input_control *controls);

void input_init();
void input_load_defaults();

bool input_bindings_conflict(input_control *x, input_control *y);

// maps an SDL event to a logical input event and updates global state
void input_get_event(SDL_Event *e, input_event *mapped);

bool input_joystick_update(SDL_Event *event);
bool input_joystick_connected();
// input_joystick_num_axes returns the number of axes or 0 if no controller is connected.
int input_joystick_num_axes();
// input_joystick_axis_pos returns the position of the given axis or 0 if no controller is connected.
Sint16 input_joystick_axis_pos(int which);

void input_destroy();

#endif
