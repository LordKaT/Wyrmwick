#include "include.h"

static void _set_lua_constants(lua_State *st);
static int _save_controls(FILE *sfile, void *controls);

static void _append_control(input_control* controls, input_control ctl);
static int _lua_input_key(lua_State *L);
static int _lua_input_joy_button(lua_State *L);
static int _lua_input_joy_axis(lua_State *L);

static const char* _kcname(SDL_Keycode code);
static const char* _inkname(int key);

static bool _update_key(int which, int newstate);


void input_init() {
	if (g_keybinds[0].m_type == IN_NONE) {
		// Nothing set in the settings, load defaults.
		input_load_defaults();
	}
	
	g_sdlJoystick = SDL_JoystickOpen(0);
	if (g_sdlJoystick == nullptr) {
		debug_print("No joysticks found. %s\n", SDL_GetError());
		return;
	}
	debug_print("Found a joystick: %s\n", SDL_JoystickName(g_sdlJoystick));
	SDL_JoystickEventState(SDL_ENABLE);
}

void input_destroy() {
	if (g_sdlJoystick != nullptr && SDL_JoystickGetAttached(g_sdlJoystick)) {
		SDL_JoystickClose(g_sdlJoystick);
	}
}

bool input_bindings_conflict(input_control *x, input_control *y) {
	if (x->m_type != y->m_type) { return false; }
	switch (x->m_type) {
	case IN_TYPE_KEYBOARD:
		return x->m_keycode == y->m_keycode;
		break;
	case IN_TYPE_JOYBUTTON:
		return x->m_iIndex == y->m_iIndex;
		break;
	case IN_TYPE_JOYAXIS:
		if (x->m_iIndex != y->m_iIndex) { return false; }
		if (x->m_iAxisDir > 0 && y->m_iAxisDir < 0) { return false; }
		if (x->m_iAxisDir < 0 && y->m_iAxisDir > 0) { return false; }
		return true;
		break;
	default:
		break;
	}
	return false;
}

void input_config_settings(settings *st, input_control *controls) {
	controls[0].m_type = IN_NONE;
	
	_set_lua_constants(st->m_luaState);
	
	settings_add_func(st, "input_key", &_lua_input_key, controls);
	settings_add_func(st, "input_joy_button", &_lua_input_joy_button, controls);
	settings_add_func(st, "input_joy_axis", &_lua_input_joy_axis, controls);
	
	settings_add_writer(st, &_save_controls, controls);
}

int _save_controls(FILE *sfile, void *controls) {
	input_control *ctl = (input_control*) controls;
	
	for (int i = 0; i < IN_MAX; i++) {
		if (ctl[i].m_type == IN_NONE) { break; }
		switch (ctl[i].m_type) {
			case IN_TYPE_KEYBOARD:
				fprintf(sfile, "input_key(%s, %s)\n", _kcname(ctl[i].m_keycode), _inkname(ctl[i].m_iTo));
				break;
			case IN_TYPE_JOYBUTTON:
				fprintf(sfile, "input_joy_button(%d, %s)\n", ctl[i].m_iIndex, _inkname(ctl[i].m_iTo));
				break;
			case IN_TYPE_JOYAXIS:
				fprintf(sfile, "input_joy_axis(%d, %d, %s)\n",
					ctl[i].m_iIndex, ctl[i].m_iAxisDir, _inkname(ctl[i].m_iTo));
				break;
		}
	}
	return 0;
}

void input_load_defaults() {
	input_control m;
	m.m_type = IN_TYPE_KEYBOARD;
	
	m.m_keycode = SDLK_UP;      m.m_iTo = IN_DIRUP;     g_keybinds[0] = m;
	m.m_keycode = SDLK_DOWN;    m.m_iTo = IN_DIRDOWN;   g_keybinds[1] = m;
	m.m_keycode = SDLK_LEFT;    m.m_iTo = IN_DIRLEFT;   g_keybinds[2] = m;
	m.m_keycode = SDLK_RIGHT;   m.m_iTo = IN_DIRRIGHT;  g_keybinds[3] = m;
	m.m_keycode = SDLK_RETURN;  m.m_iTo = IN_OK;        g_keybinds[4] = m;
	
	m.m_type = IN_NONE; g_keybinds[5] = m;
}

void input_get_event(SDL_Event e, input_event *mapped) {
	mapped->m_iKey = IN_NONE;
	
	int ktype;
	switch (e.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			ktype = IN_TYPE_KEYBOARD;
			break;
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			ktype = IN_TYPE_JOYBUTTON;
			break;
		case SDL_JOYAXISMOTION:
			ktype = IN_TYPE_JOYAXIS;
			break;
		default:
			mapped->m_iType = IN_NONE;
			return;
	}
	
	for (int i = 0; i < IN_MAX; i++) {
		if (g_keybinds[i].m_type == IN_NONE) { break; }
		if (g_keybinds[i].m_type != ktype) { continue; }

		bool found = false;
		switch (ktype) {	
			case IN_TYPE_KEYBOARD:
				if (e.key.keysym.sym == g_keybinds[i].m_keycode) { found = true; }
				break;
			case IN_TYPE_JOYBUTTON:
				if (e.jbutton.button == g_keybinds[i].m_iIndex) { found = true; }
				break;
			case IN_TYPE_JOYAXIS:
				if (e.jaxis.axis == g_keybinds[i].m_iIndex) { found = true; }
				break;
		}
		if (!found) { continue; }
		
		int newstate = 0;
		switch (ktype) {	
			case IN_TYPE_KEYBOARD:
				if (e.type == SDL_KEYDOWN) { newstate = g_keybinds[i].m_iTo; }
				break;
			case IN_TYPE_JOYBUTTON:
				if (e.type == SDL_JOYBUTTONDOWN) { newstate = g_keybinds[i].m_iTo; }
				break;
			case IN_TYPE_JOYAXIS:
				// TODO: Hysteresis?
				if (e.jaxis.value * g_keybinds[i].m_iAxisDir > 1000) { newstate = g_keybinds[i].m_iTo; }
				break;
		}
		
		bool changed = _update_key(g_keybinds[i].m_iTo, newstate);
		if (!changed) {
			continue;
		}
		
		if ((g_keybinds[i].m_iTo & (IN_DIRUP|IN_DIRDOWN|IN_DIRLEFT|IN_DIRRIGHT)) == 0) {
			mapped->m_iType = newstate != 0 ? IN_ON : IN_OFF;
			mapped->m_iKey = g_keybinds[i].m_iTo;
		} else {
			mapped->m_iType = IN_DIRCHANGE;
			mapped->m_iKey = g_input_state.m_iDirection;
		}
	}
}

bool input_joystick_connected() {
	if (g_sdlJoystick) {
		return true;
	}
	
	if (SDL_NumJoysticks() != 0) {
		g_sdlJoystick = SDL_JoystickOpen(0);
		SDL_JoystickEventState(SDL_ENABLE);
		return true;
	}
	
	return false;
}

int input_joystick_num_axes() {
	if (! g_sdlJoystick) {
		return 0;
	}
	
	int n = SDL_JoystickNumAxes(g_sdlJoystick);
	if (n < 0) { return 0; }
	return n;
}

Sint16 input_joystick_axis_pos(int which) {
	if (! g_sdlJoystick) {
		return 0;
	}
	
	return SDL_JoystickGetAxis(g_sdlJoystick, which);
}

bool _update_key(int which, int newstate) {
	int oldstate = which & g_input_state.m_iActive;
	
	if (newstate == oldstate) {
		return false;
	}
	
	g_input_state.m_iActive &= ~which;
	g_input_state.m_iActive |= newstate;
	
	if ((which & (IN_DIRUP|IN_DIRDOWN|IN_DIRLEFT|IN_DIRRIGHT)) == 0) {
		return true;
	}
	
	if (newstate != 0) {
		g_input_state.m_iDirection = which;
		return true;
	}
	
	if (which != g_input_state.m_iDirection) {
		return false;
	}
	
	if (g_input_state.m_iActive & IN_DIRRIGHT) {
		g_input_state.m_iDirection = IN_DIRRIGHT;
		return true;
	}
	if (g_input_state.m_iActive & IN_DIRLEFT) {
		g_input_state.m_iDirection = IN_DIRLEFT;
		return true;
	}
	if (g_input_state.m_iActive & IN_DIRUP) {
		g_input_state.m_iDirection = IN_DIRUP;
		return true;
	}
	if (g_input_state.m_iActive & IN_DIRDOWN) {
		g_input_state.m_iDirection = IN_DIRDOWN;
		return true;
	}
	
	g_input_state.m_iDirection = IN_NONE;
	return true;
}

void _append_control(input_control* controls, input_control ctl) {
	int i;
	for (i = 0; i < IN_MAX-1; i++) {
		if (controls[i].m_type == IN_NONE) {
			controls[i] = ctl;
			controls[i+1].m_type = IN_NONE;
			return;
		}
		
		if (controls[i].m_type != ctl.m_type) { continue; }
		switch (ctl.m_type) {
			case IN_TYPE_KEYBOARD:
				if (controls[i].m_keycode == ctl.m_keycode) {
					controls[i] = ctl;
					return;
				}
				break;
			case IN_TYPE_JOYBUTTON:
				if (controls[i].m_iIndex == ctl.m_iIndex) {
					controls[i] = ctl;
					return;
				}
				break;
			
			case IN_TYPE_JOYAXIS:
				if (controls[i].m_iIndex == ctl.m_iIndex && controls[i].m_iAxisDir == ctl.m_iAxisDir) {
					controls[i] = ctl;
					return;
				}
				break;
		}
	}
}

int _lua_input_key(lua_State *L) {
	if (lua_gettop(L) != 2) {
		lua_pushstring(L, "function input_key needs exactly 2 arguments");
		lua_error(L);
		return 0;
	}
	
	if ((! lua_isnumber(L, 1)) || (! lua_isnumber(L, 2))) {
		lua_pushstring(L, "arguments of function input_key must be (number, number)");
		lua_error(L);
		return 0;
	}
	
	input_control *controls = (input_control*) lua_touserdata(L, lua_upvalueindex(1));
	
	int from = lua_tointeger(L, 1);
	int to   = lua_tointeger(L, 2);
	input_control m = {IN_TYPE_KEYBOARD, 0, 0, (SDL_Keycode) from, to};
	_append_control(controls, m);
	return 0;
}

int _lua_input_joy_button(lua_State *L) {
	if (lua_gettop(L) != 2) {
		lua_pushstring(L, "function input_joy_button needs exactly 2 arguments");
		lua_error(L);
		return 0;
	}
	
	if ((! lua_isnumber(L, 1)) || (! lua_isnumber(L, 2))) {
		lua_pushstring(L, "arguments of function input_joy_button must be (number, number)");
		lua_error(L);
		return 0;
	}
	
	input_control *controls = (input_control*) lua_touserdata(L, lua_upvalueindex(1));
	
	int from = lua_tointeger(L, 1);
	int to   = lua_tointeger(L, 2);
	input_control m = {IN_TYPE_JOYBUTTON, (Uint8) from, 0, 0, to};
	_append_control(controls, m);
	return 0;
}

int _lua_input_joy_axis(lua_State *L) {
	if (lua_gettop(L) != 3) {
		lua_pushstring(L, "function input_joy_axis needs exactly 3 arguments");
		lua_error(L);
		return 0;
	}
	
	if ((! lua_isnumber(L, 1)) || (! lua_isnumber(L, 2)) || (! lua_isnumber(L, 3))) {
		lua_pushstring(L, "arguments of function input_joy_axis must be (number, number, number)");
		lua_error(L);
		return 0;
	}
	
	input_control *controls = (input_control*) lua_touserdata(L, lua_upvalueindex(1));
	
	int axis = lua_tointeger(L, 1);
	int dir  = lua_tointeger(L, 2);
	int to   = lua_tointeger(L, 3);
	input_control m = {IN_TYPE_JOYAXIS, (Uint8) axis, dir, 0, to};
	_append_control(controls, m);
	return 0;
}

const struct _keycode {
	const char *name;
	SDL_Keycode code;
} _keycodes[] = {
	{"SDLK_UNKNOWN", SDLK_UNKNOWN},
	{"SDLK_RETURN", SDLK_RETURN},
	{"SDLK_ESCAPE", SDLK_ESCAPE},
	{"SDLK_BACKSPACE", SDLK_BACKSPACE},
	{"SDLK_TAB", SDLK_TAB},
	{"SDLK_SPACE", SDLK_SPACE},
	{"SDLK_EXCLAIM", SDLK_EXCLAIM},
	{"SDLK_QUOTEDBL", SDLK_QUOTEDBL},
	{"SDLK_HASH", SDLK_HASH},
	{"SDLK_PERCENT", SDLK_PERCENT},
	{"SDLK_DOLLAR", SDLK_DOLLAR},
	{"SDLK_AMPERSAND", SDLK_AMPERSAND},
	{"SDLK_QUOTE", SDLK_QUOTE},
	{"SDLK_LEFTPAREN", SDLK_LEFTPAREN},
	{"SDLK_RIGHTPAREN", SDLK_RIGHTPAREN},
	{"SDLK_ASTERISK", SDLK_ASTERISK},
	{"SDLK_PLUS", SDLK_PLUS},
	{"SDLK_COMMA", SDLK_COMMA},
	{"SDLK_MINUS", SDLK_MINUS},
	{"SDLK_PERIOD", SDLK_PERIOD},
	{"SDLK_SLASH", SDLK_SLASH},
	{"SDLK_0", SDLK_0},
	{"SDLK_1", SDLK_1},
	{"SDLK_2", SDLK_2},
	{"SDLK_3", SDLK_3},
	{"SDLK_4", SDLK_4},
	{"SDLK_5", SDLK_5},
	{"SDLK_6", SDLK_6},
	{"SDLK_7", SDLK_7},
	{"SDLK_8", SDLK_8},
	{"SDLK_9", SDLK_9},
	{"SDLK_COLON", SDLK_COLON},
	{"SDLK_SEMICOLON", SDLK_SEMICOLON},
	{"SDLK_LESS", SDLK_LESS},
	{"SDLK_EQUALS", SDLK_EQUALS},
	{"SDLK_GREATER", SDLK_GREATER},
	{"SDLK_QUESTION", SDLK_QUESTION},
	{"SDLK_AT", SDLK_AT},
	{"SDLK_LEFTBRACKET", SDLK_LEFTBRACKET},
	{"SDLK_BACKSLASH", SDLK_BACKSLASH},
	{"SDLK_RIGHTBRACKET", SDLK_RIGHTBRACKET},
	{"SDLK_CARET", SDLK_CARET},
	{"SDLK_UNDERSCORE", SDLK_UNDERSCORE},
	{"SDLK_BACKQUOTE", SDLK_BACKQUOTE},
	{"SDLK_a", SDLK_a},
	{"SDLK_b", SDLK_b},
	{"SDLK_c", SDLK_c},
	{"SDLK_d", SDLK_d},
	{"SDLK_e", SDLK_e},
	{"SDLK_f", SDLK_f},
	{"SDLK_g", SDLK_g},
	{"SDLK_h", SDLK_h},
	{"SDLK_i", SDLK_i},
	{"SDLK_j", SDLK_j},
	{"SDLK_k", SDLK_k},
	{"SDLK_l", SDLK_l},
	{"SDLK_m", SDLK_m},
	{"SDLK_n", SDLK_n},
	{"SDLK_o", SDLK_o},
	{"SDLK_p", SDLK_p},
	{"SDLK_q", SDLK_q},
	{"SDLK_r", SDLK_r},
	{"SDLK_s", SDLK_s},
	{"SDLK_t", SDLK_t},
	{"SDLK_u", SDLK_u},
	{"SDLK_v", SDLK_v},
	{"SDLK_w", SDLK_w},
	{"SDLK_x", SDLK_x},
	{"SDLK_y", SDLK_y},
	{"SDLK_z", SDLK_z},
	{"SDLK_CAPSLOCK", SDLK_CAPSLOCK},
	{"SDLK_F1", SDLK_F1},
	{"SDLK_F2", SDLK_F2},
	{"SDLK_F3", SDLK_F3},
	{"SDLK_F4", SDLK_F4},
	{"SDLK_F5", SDLK_F5},
	{"SDLK_F6", SDLK_F6},
	{"SDLK_F7", SDLK_F7},
	{"SDLK_F8", SDLK_F8},
	{"SDLK_F9", SDLK_F9},
	{"SDLK_F10", SDLK_F10},
	{"SDLK_F11", SDLK_F11},
	{"SDLK_F12", SDLK_F12},
	{"SDLK_PRINTSCREEN", SDLK_PRINTSCREEN},
	{"SDLK_SCROLLLOCK", SDLK_SCROLLLOCK},
	{"SDLK_PAUSE", SDLK_PAUSE},
	{"SDLK_INSERT", SDLK_INSERT},
	{"SDLK_HOME", SDLK_HOME},
	{"SDLK_PAGEUP", SDLK_PAGEUP},
	{"SDLK_DELETE", SDLK_DELETE},
	{"SDLK_END", SDLK_END},
	{"SDLK_PAGEDOWN", SDLK_PAGEDOWN},
	{"SDLK_RIGHT", SDLK_RIGHT},
	{"SDLK_LEFT", SDLK_LEFT},
	{"SDLK_DOWN", SDLK_DOWN},
	{"SDLK_UP", SDLK_UP},
	{"SDLK_NUMLOCKCLEAR", SDLK_NUMLOCKCLEAR},
	{"SDLK_KP_DIVIDE", SDLK_KP_DIVIDE},
	{"SDLK_KP_MULTIPLY", SDLK_KP_MULTIPLY},
	{"SDLK_KP_MINUS", SDLK_KP_MINUS},
	{"SDLK_KP_PLUS", SDLK_KP_PLUS},
	{"SDLK_KP_ENTER", SDLK_KP_ENTER},
	{"SDLK_KP_1", SDLK_KP_1},
	{"SDLK_KP_2", SDLK_KP_2},
	{"SDLK_KP_3", SDLK_KP_3},
	{"SDLK_KP_4", SDLK_KP_4},
	{"SDLK_KP_5", SDLK_KP_5},
	{"SDLK_KP_6", SDLK_KP_6},
	{"SDLK_KP_7", SDLK_KP_7},
	{"SDLK_KP_8", SDLK_KP_8},
	{"SDLK_KP_9", SDLK_KP_9},
	{"SDLK_KP_0", SDLK_KP_0},
	{"SDLK_KP_PERIOD", SDLK_KP_PERIOD},
	{"SDLK_APPLICATION", SDLK_APPLICATION},
	{"SDLK_POWER", SDLK_POWER},
	{"SDLK_KP_EQUALS", SDLK_KP_EQUALS},
	{"SDLK_F13", SDLK_F13},
	{"SDLK_F14", SDLK_F14},
	{"SDLK_F15", SDLK_F15},
	{"SDLK_F16", SDLK_F16},
	{"SDLK_F17", SDLK_F17},
	{"SDLK_F18", SDLK_F18},
	{"SDLK_F19", SDLK_F19},
	{"SDLK_F20", SDLK_F20},
	{"SDLK_F21", SDLK_F21},
	{"SDLK_F22", SDLK_F22},
	{"SDLK_F23", SDLK_F23},
	{"SDLK_F24", SDLK_F24},
	{"SDLK_EXECUTE", SDLK_EXECUTE},
	{"SDLK_HELP", SDLK_HELP},
	{"SDLK_MENU", SDLK_MENU},
	{"SDLK_SELECT", SDLK_SELECT},
	{"SDLK_STOP", SDLK_STOP},
	{"SDLK_AGAIN", SDLK_AGAIN},
	{"SDLK_UNDO", SDLK_UNDO},
	{"SDLK_CUT", SDLK_CUT},
	{"SDLK_COPY", SDLK_COPY},
	{"SDLK_PASTE", SDLK_PASTE},
	{"SDLK_FIND", SDLK_FIND},
	{"SDLK_MUTE", SDLK_MUTE},
	{"SDLK_VOLUMEUP", SDLK_VOLUMEUP},
	{"SDLK_VOLUMEDOWN", SDLK_VOLUMEDOWN},
	{"SDLK_KP_COMMA", SDLK_KP_COMMA},
	{"SDLK_KP_EQUALSAS400", SDLK_KP_EQUALSAS400},
	{"SDLK_ALTERASE", SDLK_ALTERASE},
	{"SDLK_SYSREQ", SDLK_SYSREQ},
	{"SDLK_CANCEL", SDLK_CANCEL},
	{"SDLK_CLEAR", SDLK_CLEAR},
	{"SDLK_PRIOR", SDLK_PRIOR},
	{"SDLK_RETURN2", SDLK_RETURN2},
	{"SDLK_SEPARATOR", SDLK_SEPARATOR},
	{"SDLK_OUT", SDLK_OUT},
	{"SDLK_OPER", SDLK_OPER},
	{"SDLK_CLEARAGAIN", SDLK_CLEARAGAIN},
	{"SDLK_CRSEL", SDLK_CRSEL},
	{"SDLK_EXSEL", SDLK_EXSEL},
	{"SDLK_KP_00", SDLK_KP_00},
	{"SDLK_KP_000", SDLK_KP_000},
	{"SDLK_THOUSANDSSEPARATOR", SDLK_THOUSANDSSEPARATOR},
	{"SDLK_DECIMALSEPARATOR", SDLK_DECIMALSEPARATOR},
	{"SDLK_CURRENCYUNIT", SDLK_CURRENCYUNIT},
	{"SDLK_CURRENCYSUBUNIT", SDLK_CURRENCYSUBUNIT},
	{"SDLK_KP_LEFTPAREN", SDLK_KP_LEFTPAREN},
	{"SDLK_KP_RIGHTPAREN", SDLK_KP_RIGHTPAREN},
	{"SDLK_KP_LEFTBRACE", SDLK_KP_LEFTBRACE},
	{"SDLK_KP_RIGHTBRACE", SDLK_KP_RIGHTBRACE},
	{"SDLK_KP_TAB", SDLK_KP_TAB},
	{"SDLK_KP_BACKSPACE", SDLK_KP_BACKSPACE},
	{"SDLK_KP_A", SDLK_KP_A},
	{"SDLK_KP_B", SDLK_KP_B},
	{"SDLK_KP_C", SDLK_KP_C},
	{"SDLK_KP_D", SDLK_KP_D},
	{"SDLK_KP_E", SDLK_KP_E},
	{"SDLK_KP_F", SDLK_KP_F},
	{"SDLK_KP_XOR", SDLK_KP_XOR},
	{"SDLK_KP_POWER", SDLK_KP_POWER},
	{"SDLK_KP_PERCENT", SDLK_KP_PERCENT},
	{"SDLK_KP_LESS", SDLK_KP_LESS},
	{"SDLK_KP_GREATER", SDLK_KP_GREATER},
	{"SDLK_KP_AMPERSAND", SDLK_KP_AMPERSAND},
	{"SDLK_KP_DBLAMPERSAND", SDLK_KP_DBLAMPERSAND},
	{"SDLK_KP_VERTICALBAR", SDLK_KP_VERTICALBAR},
	{"SDLK_KP_DBLVERTICALBAR", SDLK_KP_DBLVERTICALBAR},
	{"SDLK_KP_COLON", SDLK_KP_COLON},
	{"SDLK_KP_HASH", SDLK_KP_HASH},
	{"SDLK_KP_SPACE", SDLK_KP_SPACE},
	{"SDLK_KP_AT", SDLK_KP_AT},
	{"SDLK_KP_EXCLAM", SDLK_KP_EXCLAM},
	{"SDLK_KP_MEMSTORE", SDLK_KP_MEMSTORE},
	{"SDLK_KP_MEMRECALL", SDLK_KP_MEMRECALL},
	{"SDLK_KP_MEMCLEAR", SDLK_KP_MEMCLEAR},
	{"SDLK_KP_MEMADD", SDLK_KP_MEMADD},
	{"SDLK_KP_MEMSUBTRACT", SDLK_KP_MEMSUBTRACT},
	{"SDLK_KP_MEMMULTIPLY", SDLK_KP_MEMMULTIPLY},
	{"SDLK_KP_MEMDIVIDE", SDLK_KP_MEMDIVIDE},
	{"SDLK_KP_PLUSMINUS", SDLK_KP_PLUSMINUS},
	{"SDLK_KP_CLEAR", SDLK_KP_CLEAR},
	{"SDLK_KP_CLEARENTRY", SDLK_KP_CLEARENTRY},
	{"SDLK_KP_BINARY", SDLK_KP_BINARY},
	{"SDLK_KP_OCTAL", SDLK_KP_OCTAL},
	{"SDLK_KP_DECIMAL", SDLK_KP_DECIMAL},
	{"SDLK_KP_HEXADECIMAL", SDLK_KP_HEXADECIMAL},
	{"SDLK_LCTRL", SDLK_LCTRL},
	{"SDLK_LSHIFT", SDLK_LSHIFT},
	{"SDLK_LALT", SDLK_LALT},
	{"SDLK_LGUI", SDLK_LGUI},
	{"SDLK_RCTRL", SDLK_RCTRL},
	{"SDLK_RSHIFT", SDLK_RSHIFT},
	{"SDLK_RALT", SDLK_RALT},
	{"SDLK_RGUI", SDLK_RGUI},
	{"SDLK_MODE", SDLK_MODE},
	{"SDLK_AUDIONEXT", SDLK_AUDIONEXT},
	{"SDLK_AUDIOPREV", SDLK_AUDIOPREV},
	{"SDLK_AUDIOSTOP", SDLK_AUDIOSTOP},
	{"SDLK_AUDIOPLAY", SDLK_AUDIOPLAY},
	{"SDLK_AUDIOMUTE", SDLK_AUDIOMUTE},
	{"SDLK_MEDIASELECT", SDLK_MEDIASELECT},
	{"SDLK_WWW", SDLK_WWW},
	{"SDLK_MAIL", SDLK_MAIL},
	{"SDLK_CALCULATOR", SDLK_CALCULATOR},
	{"SDLK_COMPUTER", SDLK_COMPUTER},
	{"SDLK_AC_SEARCH", SDLK_AC_SEARCH},
	{"SDLK_AC_HOME", SDLK_AC_HOME},
	{"SDLK_AC_BACK", SDLK_AC_BACK},
	{"SDLK_AC_FORWARD", SDLK_AC_FORWARD},
	{"SDLK_AC_STOP", SDLK_AC_STOP},
	{"SDLK_AC_REFRESH", SDLK_AC_REFRESH},
	{"SDLK_AC_BOOKMARKS", SDLK_AC_BOOKMARKS},
	{"SDLK_BRIGHTNESSDOWN", SDLK_BRIGHTNESSDOWN},
	{"SDLK_BRIGHTNESSUP", SDLK_BRIGHTNESSUP},
	{"SDLK_DISPLAYSWITCH", SDLK_DISPLAYSWITCH},
	{"SDLK_KBDILLUMTOGGLE", SDLK_KBDILLUMTOGGLE},
	{"SDLK_KBDILLUMDOWN", SDLK_KBDILLUMDOWN},
	{"SDLK_KBDILLUMUP", SDLK_KBDILLUMUP},
	{"SDLK_EJECT", SDLK_EJECT},
	{"SDLK_SLEEP", SDLK_SLEEP},
};

const struct _inkeyname {
	const char* name;
	int key;
} _in_key_names[] = {
	{"UP",    IN_DIRUP},
	{"DOWN",  IN_DIRDOWN},
	{"LEFT",  IN_DIRLEFT},
	{"RIGHT", IN_DIRRIGHT},
	{"OK",    IN_OK},
};

void _set_lua_constants(lua_State *L) {
	for (int i = 0; i < SIZE(_keycodes); i++) {
		lua_pushnumber(L, (lua_Number) _keycodes[i].code);
		lua_setglobal(L, _keycodes[i].name);
	}
	
	for (int i = 0; i < SIZE(_in_key_names); i++) {
		lua_pushnumber(L, (lua_Number) _in_key_names[i].key);
		lua_setglobal(L, _in_key_names[i].name);
	}
}

const char* _kcname(SDL_Keycode code) {
	for (int i = 0; i < SIZE(_keycodes); i++) {
		if (_keycodes[i].code == code) { return _keycodes[i].name; }
	}
	return "SDLK_UNKNOWN";
}

const char* _inkname(int key) {
	for (int i = 0; i < SIZE(_in_key_names); i++) {
		if (_in_key_names[i].key == key) { return _in_key_names[i].name; }
	}
	return nullptr;
}
