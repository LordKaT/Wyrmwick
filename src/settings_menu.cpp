#include "include.h"

#define JOY_AXIS_THRESHOLD 20

struct _settings_menu {
	menu *m_menu;
	settings *m_settings;
};

struct _controls_menu {
	menu *m_cmdMenu;
	menu *m_bindingsMenu;
	int m_iWhichBinding;
	input_control m_aControls[5];
};

static void _init(state_stack* stack);
static void _event(state_stack* stack, SDL_Event *sdlEvent);
static void _draw(state_stack* stack);
static void _destroy(state_stack* stack);

static void _controls_push(state_stack* stack);
static void _init_controls(state_stack* stack);
static void _event_controls(state_stack* stack, SDL_Event *sdlEvent);
static void _draw_controls(state_stack* stack);
static void _destroy_controls(state_stack* stack);

static void _bind(_controls_menu *data, SDL_Event *sdlEvent);
static void _update_bind_desc(_controls_menu *data);
static char* _bind_desc(input_control *bind);
static void _apply_bindings(_controls_menu *data);
static input_control _find_control_bind(int which);

void settings_menu_push(state_stack* stack) {
	state_desc mm = {
		GAME_SETTINGS_MENU, nullptr,
		&_init,
		nullptr, nullptr,
		&_event,
		nullptr,
		&_draw,
		&_destroy,
		nullptr, false,
	};
	table_append(stack, &mm);
}

void _init(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_settings_menu *data = (_settings_menu*) malloc(sizeof(_settings_menu));
	top->m_pData = data;
	
	menu *pMenu = menu_init(100, 50);
	menu_add_entry(pMenu, "Controls");
	menu_add_entry(pMenu, "");
	menu_add_entry(pMenu, STR_ARROW_LEFT " Save Settings");
	menu_add_entry(pMenu, STR_ARROW_LEFT " Discard Changes");
	menu_auto_resize(pMenu);
	data->m_menu = pMenu;
	
	data->m_settings = settings_init();
	input_config_settings(data->m_settings, g_keybinds);
	screen_config_settings(data->m_settings, nullptr);
	settings_load(data->m_settings, settings_file_path);
}

void _event(state_stack* stack, SDL_Event *sdlEvent) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_settings_menu *data = (_settings_menu*) top->m_pData;
	menu *pMenu = (menu*) data->m_menu;
	settings *pSettings = (settings*) data->m_settings;
	
	switch (menu_input(pMenu, sdlEvent)) {
	case 0:
		top->m_fnPushChild = &_controls_push;
		break;
	case 1:
		break;
	case 2:
		settings_save(pSettings, settings_file_path);
		top->m_isDead = true;
		break;
	case 3:
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


void _controls_push(state_stack* stack) {
	state_desc mm = {
		GAME_SETTINGS_CONTROLS, nullptr,
		&_init_controls,
		nullptr, nullptr,
		&_event_controls,
		nullptr,
		&_draw_controls,
		&_destroy_controls,
		nullptr, false,
	};
	table_append(stack, &mm);
}

void _init_controls(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_controls_menu *data = (_controls_menu*) malloc(sizeof(_controls_menu));
	top->m_pData = data;
	
	data->m_iWhichBinding = -1;
	
	menu *pCmdMenu = menu_init(200, 50);
	menu_add_entry(pCmdMenu, "Set Controls");
	menu_add_entry(pCmdMenu, "Restore Defaults");
	menu_add_entry(pCmdMenu, STR_ARROW_LEFT " Back");
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
}

// TODO: Prevent mapping the same button multiple times!!
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
		if (sdlEvent->jaxis.value > -JOY_AXIS_THRESHOLD && sdlEvent->jaxis.value < JOY_AXIS_THRESHOLD) {
			return;
		}
		control->m_type = IN_TYPE_JOYAXIS;
		control->m_iIndex = sdlEvent->jaxis.axis;
		control->m_iAxisDir = sdlEvent->jaxis.value<0 ? -1 : 1;
		break;
	default:
		return;
	}
	
	data->m_iWhichBinding++;
	data->m_bindingsMenu->m_iCursorPos++;
}

static void _event_controls(state_stack* stack, SDL_Event *sdlEvent) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_controls_menu *data = (_controls_menu*) top->m_pData;
	
	if (data->m_iWhichBinding >= 0) {
		_bind(data, sdlEvent);
		_update_bind_desc(data);
		
		if (data->m_iWhichBinding > 4) {
			data->m_iWhichBinding = -1;
			data->m_cmdMenu->m_iCursorPos = 0;
			_apply_bindings(data);
		}
	}
	
	switch (menu_input(data->m_cmdMenu, sdlEvent)) {
	case 0:
		data->m_iWhichBinding = 0;
		data->m_cmdMenu->m_iCursorPos = -1;
		data->m_bindingsMenu->m_iCursorPos = 0;
		break;
	case 1:
		data->m_aControls[0] = {IN_TYPE_KEYBOARD, 0, 0, SDLK_UP,     IN_DIRUP};
		data->m_aControls[1] = {IN_TYPE_KEYBOARD, 0, 0, SDLK_DOWN,   IN_DIRDOWN};
		data->m_aControls[2] = {IN_TYPE_KEYBOARD, 0, 0, SDLK_LEFT,   IN_DIRLEFT};
		data->m_aControls[3] = {IN_TYPE_KEYBOARD, 0, 0, SDLK_RIGHT,  IN_DIRRIGHT};
		data->m_aControls[4] = {IN_TYPE_KEYBOARD, 0, 0, SDLK_RETURN, IN_OK};
		_update_bind_desc(data);
		_apply_bindings(data);
		break;
	case 2:
		top->m_isDead = true;
		break;
	}
}

static void _draw_controls(state_stack* stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_controls_menu *data = (_controls_menu*) top->m_pData;
	
	menu_render(data->m_cmdMenu);
	menu_render(data->m_bindingsMenu);
}

static void _destroy_controls(state_stack* stack) {
}

void _update_bind_desc(_controls_menu *data) {
	for (int i = 0; i < 5; i++) {
		char *desc;
		table_get(data->m_bindingsMenu->m_aValues, i, &desc);
		free(desc);
		desc = _bind_desc(&(data->m_aControls[i]));
		table_put(data->m_bindingsMenu->m_aValues, i, &desc);
	}
}

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
