#include "include.h"

#define JOY_AXIS_THRESHOLD_LOW 5
#define JOY_AXIS_THRESHOLD_HIGH 10000

struct _settings_menu {
	menu *m_menu;
	settings *m_settings;
};

struct _controls_menu {
	menu *m_cmdMenu;
	menu *m_bindingsMenu;
	int m_iWhichBinding;
	input_control m_aControls[5];
	bool m_capturingBindings;
	bool m_bindingsConflict;
	
	int m_iJoyAxes;
	// This is used to ignore the axis that was just bound until it's centered again.
	// Otherwise, all of the controls would get mapped to one axis with analog sticks.
	bool *m_bIgnoreAxes;
};

static void _event(state_stack* stack, SDL_Event *sdlEvent);
static void _draw(state_stack* stack);
static void _destroy(state_stack* stack);

static void _controls_push(state_stack* stack, void *udata);
static void _event_controls(state_stack* stack, SDL_Event *sdlEvent);
static void _draw_controls(state_stack* stack);
static void _destroy_controls(state_stack* stack);

static void _bind(_controls_menu *data, SDL_Event *sdlEvent);
static void _update_bind_desc(_controls_menu *data);
static char* _bind_desc(input_control *bind);
static void _apply_bindings(_controls_menu *data);
static input_control _find_control_bind(int which);

void settings_menu_push(state_stack* stack, void* udata) {
	(void) udata;
	state_desc mm = {
		GAME_SETTINGS_MENU, nullptr,
		nullptr, nullptr,
		&_event,
		nullptr,
		&_draw,
		&_destroy,
		nullptr, nullptr, false,
	};
	
	_settings_menu *data = (_settings_menu*) malloc(sizeof(_settings_menu));
	
	menu *pMenu = menu_init(100, 50);
	menu_add_entry(pMenu, "Controls");
	menu_add_entry(pMenu, "");
	menu_add_entry(pMenu, STR_ARROWBIG_LEFT " Save Settings");
	menu_add_entry(pMenu, STR_ARROWBIG_LEFT " Discard Changes");
	menu_auto_resize(pMenu);
	data->m_menu = pMenu;
	
	data->m_settings = settings_init();
	input_config_settings(data->m_settings, g_keybinds);
	screen_config_settings(data->m_settings, nullptr);
	settings_load(data->m_settings, settings_file_path);
	
	mm.m_pData = data;
	table_append(stack, &mm);
}

void _event(state_stack* stack, SDL_Event *sdlEvent) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_settings_menu *data = (_settings_menu*) top->m_pData;
	menu *pMenu = (menu*) data->m_menu;
	settings *pSettings = (settings*) data->m_settings;
	
	if (sdlEvent->type == SDL_QUIT) {
		state_stack_kill(stack);
		return;
	}
	
	switch (menu_input(pMenu, sdlEvent)) {
	case 0:
		// Controls
		top->m_fnPushChild = &_controls_push;
		break;
	case 1:
		// (empty)
		break;
	case 2:
		// < Save Settings
		settings_save(pSettings, settings_file_path);
		top->m_isDead = true;
		break;
	case 3:
		// < Discard Settings
		settings_load(pSettings, settings_file_path);
		top->m_isDead = true;
		break;
	default:
		break;
	}
}

void _draw(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_settings_menu *data = (_settings_menu*) top->m_pData;
	menu *pMenu = (menu*) data->m_menu;
	menu_render(pMenu);
}

void _destroy(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_settings_menu *data = (_settings_menu*) top->m_pData;
	menu *pMenu = (menu*) data->m_menu;
	settings *pSettings = (settings*) data->m_settings;
	
	menu_destroy(pMenu);
	settings_destroy(pSettings);
	free(data);
}


void _controls_push(state_stack* stack, void* udata) {
	(void) udata;
	state_desc mm = {
		GAME_SETTINGS_CONTROLS, nullptr,
		nullptr, nullptr,
		&_event_controls,
		nullptr,
		&_draw_controls,
		&_destroy_controls,
		nullptr, nullptr, false,
	};
	
	_controls_menu *data = (_controls_menu*) malloc(sizeof(_controls_menu));
	
	data->m_iWhichBinding = -1;
	
	menu *pCmdMenu = menu_init(200, 50);
	menu_add_entry(pCmdMenu, "Set Controls");
	menu_add_entry(pCmdMenu, "Restore Defaults");
	menu_add_entry(pCmdMenu, STR_ARROWBIG_LEFT " Back");
	menu_auto_resize(pCmdMenu);
	data->m_cmdMenu = pCmdMenu;
	
	menu *pBindMenu = menu_init(0, 0);
	pBindMenu->m_iCursorPos = -1;
	menu_add_entry(pBindMenu, STR_ARROW_UP "    ");
	menu_add_entry(pBindMenu, STR_ARROW_DOWN "    ");
	menu_add_entry(pBindMenu, STR_ARROW_LEFT "    ");
	menu_add_entry(pBindMenu, STR_ARROW_RIGHT "    ");
	menu_add_entry(pBindMenu, "OK   ");
	menu_auto_resize(pBindMenu);
	data->m_bindingsMenu = pBindMenu;
	
	pCmdMenu->m_iHeight = pBindMenu->m_iHeight;
	pBindMenu->m_iWidth *= 8;
	pBindMenu->m_iX = pCmdMenu->m_iX + pCmdMenu->m_iWidth + 6;
	pBindMenu->m_iY = pCmdMenu->m_iY;
	
	data->m_aControls[0] = _find_control_bind(IN_DIRUP);
	data->m_aControls[1] = _find_control_bind(IN_DIRDOWN);
	data->m_aControls[2] = _find_control_bind(IN_DIRLEFT);
	data->m_aControls[3] = _find_control_bind(IN_DIRRIGHT);
	data->m_aControls[4] = _find_control_bind(IN_OK);
	_update_bind_desc(data);
	
	data->m_capturingBindings = false;
	
	data->m_iJoyAxes = input_joystick_num_axes();
	data->m_bIgnoreAxes = (bool*) calloc(data->m_iJoyAxes, sizeof(bool));
	
	mm.m_pData = data;
	table_append(stack, &mm);
}

void _bind(_controls_menu *data, SDL_Event *sdlEvent) {
	input_control *control = &(data->m_aControls[data->m_iWhichBinding]);
	switch (sdlEvent->type) {
	case SDL_KEYUP:
		control->m_type = IN_TYPE_KEYBOARD;
		control->m_keycode = sdlEvent->key.keysym.sym;
		break;
	case SDL_JOYBUTTONUP:
		control->m_type = IN_TYPE_JOYBUTTON;
		control->m_iIndex = sdlEvent->jbutton.button;
		break;
	case SDL_JOYAXISMOTION:
		if (data->m_iJoyAxes <= sdlEvent->jaxis.axis) {
			free(data->m_bIgnoreAxes);
			data->m_iJoyAxes = input_joystick_num_axes();
			data->m_bIgnoreAxes = (bool*) calloc(data->m_iJoyAxes, sizeof(bool));
		}
		
		if (sdlEvent->jaxis.value > -JOY_AXIS_THRESHOLD_LOW && sdlEvent->jaxis.value < JOY_AXIS_THRESHOLD_LOW) {
			data->m_bIgnoreAxes[sdlEvent->jaxis.axis] = false;
			return;
		}
		
		if (sdlEvent->jaxis.value > -JOY_AXIS_THRESHOLD_HIGH && sdlEvent->jaxis.value < JOY_AXIS_THRESHOLD_HIGH) {
			return;
		}
		
		if (data->m_bIgnoreAxes[sdlEvent->jaxis.axis]) {
			return;
		}
		
		control->m_type = IN_TYPE_JOYAXIS;
		control->m_iIndex = sdlEvent->jaxis.axis;
		control->m_iAxisDir = sdlEvent->jaxis.value<0 ? -1 : 1;
		data->m_bIgnoreAxes[sdlEvent->jaxis.axis] = true;
		break;
	default:
		return;
	}
	
	data->m_iWhichBinding++;
	data->m_bindingsMenu->m_iCursorPos++;
}

// prev method did not compile in win.
input_control _event_create_input_control(int iType, Uint8 iIndex, int iAxisDir, SDL_Keycode keycode, int iTo) {
	input_control temp = {iType, iIndex, iAxisDir, keycode, iTo};
	return temp;
}

static void _event_controls(state_stack* stack, SDL_Event *sdlEvent) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_controls_menu *data = (_controls_menu*) top->m_pData;
	
	if (sdlEvent->type == SDL_QUIT) {
		state_stack_kill(stack);
		return;
	}
	
	if (data->m_capturingBindings) {
		_bind(data, sdlEvent);
		_update_bind_desc(data);
		
		if (data->m_iWhichBinding > 4) {
			data->m_iWhichBinding = -1;
			data->m_cmdMenu->m_iCursorPos = 0;
			data->m_capturingBindings = false;
			if (! data->m_bindingsConflict) { _apply_bindings(data); }
		}
		return;
	}
	
	switch (menu_input(data->m_cmdMenu, sdlEvent)) {
	case 0:
		// Set Controls
		data->m_iWhichBinding = 0;
		data->m_capturingBindings = true;
		break;
	case 1:
		// Restore Defaults
		data->m_aControls[0] = _event_create_input_control(IN_TYPE_KEYBOARD, 0, 0, SDLK_UP,     IN_DIRUP);
		data->m_aControls[1] = _event_create_input_control(IN_TYPE_KEYBOARD, 0, 0, SDLK_DOWN,   IN_DIRDOWN);
		data->m_aControls[2] = _event_create_input_control(IN_TYPE_KEYBOARD, 0, 0, SDLK_LEFT,   IN_DIRLEFT);
		data->m_aControls[3] = _event_create_input_control(IN_TYPE_KEYBOARD, 0, 0, SDLK_RIGHT,  IN_DIRRIGHT);
		data->m_aControls[4] = _event_create_input_control(IN_TYPE_KEYBOARD, 0, 0, SDLK_RETURN, IN_OK);
		_update_bind_desc(data);
		_apply_bindings(data);
		break;
	case 2:
		// < Back
		if (data->m_bindingsConflict) { break; }
		_apply_bindings(data);
		top->m_isDead = true;
		break;
	default:
		break;
	}
}

static void _draw_controls(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_controls_menu *data = (_controls_menu*) top->m_pData;
	
	if (data->m_capturingBindings) {
		data->m_cmdMenu->m_iCursorPos = -1;
		data->m_bindingsMenu->m_iCursorPos = data->m_iWhichBinding;
	}
	
	menu_render(data->m_cmdMenu);
	menu_render(data->m_bindingsMenu);
}

static void _destroy_controls(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_controls_menu *data = (_controls_menu*) top->m_pData;
	
	free(data->m_bIgnoreAxes);
	free(data);
}

void _update_bind_desc(_controls_menu *data) {
	Uint32 normal = COLOR_MENU_TEXT;
	for (int i = 0; i < 5; i++) {
		table_put(data->m_bindingsMenu->m_aColors, i, &normal);
	}
	table_put(data->m_cmdMenu->m_aColors, 2, &normal);
	data->m_bindingsConflict = false;
	
	// check for conflicts
	for (int i = 0; i < 4; i++) {
		for (int j = i+1; j < 5; j++) {
			if (! input_bindings_conflict(&(data->m_aControls[i]), &(data->m_aControls[j]))) { continue; }
			
			Uint32 red = 0xff0000ff, disabled = COLOR_MENU_DISABLED;
			table_put(data->m_bindingsMenu->m_aColors, i, &red);
			table_put(data->m_bindingsMenu->m_aColors, j, &red);
			table_put(data->m_cmdMenu->m_aColors, 2, &disabled);
			data->m_bindingsConflict = true;
		}
	}
	
	for (int i = 0; i < 5; i++) {
		char *desc;
		table_get(data->m_bindingsMenu->m_aValues, i, &desc);
		free(desc);
		desc = _bind_desc(&(data->m_aControls[i]));
		table_put(data->m_bindingsMenu->m_aValues, i, &desc);
	}
}

// returns a human-readable description of a cotrol binding
char* _bind_desc(input_control *bind) {
	char *buff;
	int len;
	
	switch (bind->m_type) {
	case IN_TYPE_KEYBOARD:
		return strdup(SDL_GetKeyName(bind->m_keycode));
	case IN_TYPE_JOYBUTTON:
		len = sizeof("[ joystick button XX ]");
		buff = (char*) malloc(len);
		snprintf(buff, len, "[ joystick button %d ]", bind->m_iIndex);
		return buff;
	case IN_TYPE_JOYAXIS:
		len = sizeof("[ joystick axis XX + ]");
		buff = (char*) malloc(len);
		if (bind->m_iAxisDir > 0) {
			snprintf(buff, len, "[ joystick axis %d + ]", bind->m_iIndex);
		} else {
			snprintf(buff, len, "[ joystick axis %d - ]", bind->m_iIndex);
		}
		return buff;
	}
	return nullptr;
}

void _apply_bindings(_controls_menu *data) {
	for (int i = 0; i < 5; i++) {
		g_keybinds[i] = data->m_aControls[i];
	}
	g_keybinds[5].m_type = IN_NONE;
}

// finds the binding that maps to the control given by `which`. This is needed because the controls might
// be set in a different order through the config file.
input_control _find_control_bind(int which) {
	input_control ret = {0,};
	for (int i = 0; i < IN_MAX; i++) {
		if (g_keybinds[i].m_type == IN_NONE) { break; }
		if (g_keybinds[i].m_iTo == which) {
			ret = g_keybinds[i];
			break;
		}
	}
	return ret;
}
