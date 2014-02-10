#include "include.h"

static bool _update_key(int which, int newstate);

void input_init() {
	input_load_mapping();
	
	g_sdlJoystick = SDL_JoystickOpen(0);
	if (g_sdlJoystick == nullptr) {
		debug_print("No joysticks found. %s\r\n", SDL_GetError());
		return;
	}
	debug_print("Found a joystick: %s\r\n", SDL_JoystickName(g_sdlJoystick));
	SDL_JoystickEventState(SDL_ENABLE);
}

void input_destroy() {
	if (g_sdlJoystick != nullptr && SDL_JoystickGetAttached(g_sdlJoystick)) {
		SDL_JoystickClose(g_sdlJoystick);
	}
}

void input_load_mapping() {
	// TODO: Load key mappings from a config file
	
	input_mapping m;
	m.m_type = IN_TYPE_KEYBOARD;
	
	m.m_keycode = SDLK_UP;      m.m_iTo = IN_DIRUP;     g_inmap[0] = m;
	m.m_keycode = SDLK_DOWN;    m.m_iTo = IN_DIRDOWN;   g_inmap[1] = m;
	m.m_keycode = SDLK_LEFT;    m.m_iTo = IN_DIRLEFT;   g_inmap[2] = m;
	m.m_keycode = SDLK_RIGHT;   m.m_iTo = IN_DIRRIGHT;  g_inmap[3] = m;
	m.m_keycode = SDLK_RETURN;  m.m_iTo = IN_OK;        g_inmap[4] = m;
	
	m.m_type = IN_NONE; g_inmap[5] = m;
	
	// Test map for a Logitech F310 gamepad
	/*m.m_type = IN_TYPE_JOYAXIS;
	m.m_iIndex = 0;
	m.m_iAxisDir = -1;
	m.m_iTo = IN_DIRLEFT;
	g_inmap[0] = m;
	
	m.m_type = IN_TYPE_JOYAXIS;
	m.m_iIndex = 0;
	m.m_iAxisDir = 1;
	m.m_iTo = IN_DIRRIGHT;
	g_inmap[1] = m;
	
	m.m_type = IN_TYPE_JOYAXIS;
	m.m_iIndex = 1;
	m.m_iAxisDir = -1;
	m.m_iTo = IN_DIRUP;
	g_inmap[2] = m;
	
	m.m_type = IN_TYPE_JOYAXIS;
	m.m_iIndex = 1;
	m.m_iAxisDir = 1;
	m.m_iTo = IN_DIRDOWN;
	g_inmap[3] = m;
	
	m.m_type = IN_TYPE_JOYBUTTON;
	m.m_iIndex = 0;
	m.m_iTo = IN_OK;
	g_inmap[4] = m;	
	
	m.m_type = IN_NONE;
	g_inmap[5] = m;*/
}

void input_save_mapping() {
	// TODO: Write key mappings to a config file
}

void map_input_event(SDL_Event e, input_event *mapped) {
	int ktype;
	switch(e.type) {
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
		if (g_inmap[i].m_type == IN_NONE) { break; }
		if (g_inmap[i].m_type != ktype) { continue; }

		bool found = false;
		switch(ktype) {
		case IN_TYPE_KEYBOARD:
			if (e.key.keysym.sym == g_inmap[i].m_keycode) { found = true; }
			break;
		case IN_TYPE_JOYBUTTON:
			if (e.jbutton.button == g_inmap[i].m_iIndex) { found = true; }
			break;
		case IN_TYPE_JOYAXIS:
			if (e.jaxis.axis == g_inmap[i].m_iIndex) { found = true; }
			break;
		}
		if (!found) { continue; }
		
		int newstate = 0;
		switch(ktype) {
		case IN_TYPE_KEYBOARD:
			if (e.type == SDL_KEYDOWN) { newstate = g_inmap[i].m_iTo; }
			break;
		case IN_TYPE_JOYBUTTON:
			if (e.type == SDL_JOYBUTTONDOWN) { newstate = g_inmap[i].m_iTo; }
			break;
		case IN_TYPE_JOYAXIS:
			// TODO: Still broken.
			if (e.jaxis.value * g_inmap[i].m_iAxisDir > 20) { newstate = g_inmap[i].m_iTo; }
			break;
		}
		
		bool changed = _update_key(g_inmap[i].m_iTo, newstate);
		if (!changed) {
			mapped->m_iType = IN_NONE;
			return;
		}
		
		if ((g_inmap[i].m_iTo & (IN_DIRUP|IN_DIRDOWN|IN_DIRLEFT|IN_DIRRIGHT)) == 0) {
			mapped->m_iType = newstate != 0 ? IN_ON : IN_OFF;
			mapped->m_iKey = g_inmap[i].m_iTo;
		} else {
			mapped->m_iType = IN_DIRCHANGE;
			mapped->m_iKey = g_input_state.m_iDirection;
		}
	}
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
