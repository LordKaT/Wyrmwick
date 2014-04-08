#include "include.h"

struct _talk_state {
	char *m_sNPCText;
	menu *m_pChoices;
	int m_iChoice, m_iState;
	lua_State *m_pThread;
};

// just started, waiting for any input, waiting for the user to make a choice, finished.
enum { _STARTED, _WAIT_ANY, _WAIT_CHOICE, _STOPPED};

static void _talk_push(state_stack *stack, void* udata);
static void _event(state_stack *stack, SDL_Event *evt);
static void _draw(state_stack *stack);
static void _destroy(state_stack *stack);

static int _lua_resetchoices(lua_State *L);
static int _lua_addchoice(lua_State *L);
static int _lua_offerchoice(lua_State *L);
static int _lua_wait(lua_State *L);
static int _lua_say(lua_State *L);
static int _lua_stop(lua_State *L);


// startTalking() - opens the conversation screen and returns its interface.
int talk_lua_start(lua_State *L) {
	lua_getfield(L, LUA_REGISTRYINDEX, "game/stateStack");
	if (lua_isnil(L, -1)) {
		return luaL_error(L, "game not initialized yet");
	}
	state_stack *stack = (state_stack*) lua_touserdata(L, -1);
	lua_pop(L, 1);
	
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	int i;
	if (lua_getctx(L, &i) == LUA_OK) {
		top->m_fnPushChild = _talk_push;
		top->m_pChildData = (void*) L;
		return lua_yieldk(L, 0, 0, talk_lua_start);
	}
	
	luaL_Reg funcs[] = {
		{ "resetChoices", _lua_resetchoices },
		{ "addChoice", _lua_addchoice },
		{ "offerChoice", _lua_offerchoice },
		{ "wait", _lua_wait },
		{ "say", _lua_say },
		{ "stop", _lua_stop },
		{ nullptr, nullptr },
	};
	
	luaL_newlibtable(L, funcs);
	top = (state_desc*) table_ind(stack, stack->m_len-1);
	lua_pushlightuserdata(L, top->m_pData);
	luaL_setfuncs(L, funcs, 1);
	return 1;
}

void _talk_push(state_stack *stack, void* udata) {
	state_desc  talks = {
		GAME_DIALOG, nullptr,
		nullptr, nullptr,
		&_event,
		nullptr,
		&_draw,
		&_destroy,
		nullptr, nullptr, false,
	};
	
	_talk_state *st = (_talk_state*) malloc(sizeof(_talk_state));
	st->m_sNPCText = strdup("");
	st->m_pChoices = menu_init(100, 200);
	st->m_iChoice = -1;
	st->m_iState = _STARTED;
	st->m_pThread = (lua_State*) udata;
	
	talks.m_pData = (void*) st;
	table_append(stack, &talks);
}

void _event(state_stack *stack, SDL_Event *evt) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_talk_state *st = (_talk_state*) top->m_pData;
	
	input_event mapped;
	int i, err;
	
	switch (st->m_iState) {
	case _STARTED:
		err = lua_resume(st->m_pThread, nullptr, 0);
		if (err == LUA_OK) {
			st->m_iState = _STOPPED;
			top->m_isDead = true;
			return;
		}
		
		if (err != LUA_YIELD) {
			const char *errstr = lua_tostring(st->m_pThread, -1);
			debug_print("%s\n", errstr);
			st->m_iState = _STOPPED;
			top->m_isDead = true;
			return;
		}
		_event(stack, evt);
		break;
	
	case _WAIT_CHOICE:
		i = menu_input(st->m_pChoices, evt);
		if (i == -1) { return; }
		lua_pushnumber(st->m_pThread, i);
		err = lua_resume(st->m_pThread, nullptr, 1);
		if (err == LUA_OK) {
			st->m_iState = _STOPPED;
			top->m_isDead = true;
			return;
		}
		
		if (err != LUA_YIELD) {
			const char *errstr = lua_tostring(st->m_pThread, -1);
			debug_print("%s\n", errstr);
			st->m_iState = _STOPPED;
			top->m_isDead = true;
			return;
		}
		break;
	
	case _WAIT_ANY:
		input_get_event(evt, &mapped);
		if (mapped.m_iType == IN_OFF || evt->type == SDL_MOUSEBUTTONUP) {
			err = lua_resume(st->m_pThread, nullptr, 0);
			if (err == LUA_OK) {
				st->m_iState = _STOPPED;
				top->m_isDead = true;
				return;
			}
			
			if (err != LUA_YIELD) {
				const char *errstr = lua_tostring(st->m_pThread, -1);
				debug_print("%s\n", errstr);
				st->m_iState = _STOPPED;
				top->m_isDead = true;
				return;
			}
		}
		break;
	
	case _STOPPED:
		top->m_isDead = true;
		break;
	}
}

void _draw(state_stack *stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_talk_state *st = (_talk_state*) top->m_pData;
	
	int err;
	
	switch (st->m_iState) {
	case _STARTED:
		err = lua_resume(st->m_pThread, nullptr, 0);
		if (err == LUA_OK) {
			st->m_iState = _STOPPED;
			top->m_isDead = true;
			return;
		}
		
		if (err != LUA_YIELD) {
			const char *errstr = lua_tostring(st->m_pThread, -1);
			debug_print("%s\n", errstr);
			st->m_iState = _STOPPED;
			top->m_isDead = true;
			return;
		}
		_draw(stack);
		break;
	
	case _WAIT_CHOICE:
		font_print(g_font, 100, 100, "%s", st->m_sNPCText);
		menu_render(st->m_pChoices);
		break;
	
	case _WAIT_ANY:
		font_print(g_font, 100, 100, "%s", st->m_sNPCText);
		font_print(g_font, 100, 240, "Press a button for more exposition...");
		break;
	
	case _STOPPED:
		top->m_isDead = true;
		break;
	}
}

void _destroy(state_stack *stack) {
	state_desc *top = (state_desc*) table_ind(stack, stack->m_len-1);
	_talk_state *st = (_talk_state*) top->m_pData;
	
	free(st->m_sNPCText);
	menu_destroy(st->m_pChoices);
	free(st);
}

int _lua_resetchoices(lua_State *L) {
	_talk_state *st = (_talk_state*) lua_touserdata(L, lua_upvalueindex(1));
	menu_clear(st->m_pChoices);
	return 0;
}

int _lua_addchoice(lua_State *L) {
	_talk_state *st = (_talk_state*) lua_touserdata(L, lua_upvalueindex(1));
	const char *txt = lua_tostring(L, 1);
	menu_add_entry(st->m_pChoices, txt);
	lua_remove(L, 1);
	return 0;
}

int _lua_offerchoice(lua_State *L) {
	_talk_state *st = (_talk_state*) lua_touserdata(L, lua_upvalueindex(1));
	int ctx, status;
	
	status = lua_getctx(L, &ctx);
	if (status == LUA_YIELD) {
		return 1;  // choice should be pushed on the stack before resuming
	}
	
	if (status != LUA_OK) { sys_abort(); }

	menu_auto_resize(st->m_pChoices);
	st->m_iState = _WAIT_CHOICE;
	return lua_yieldk(L, 0, 0, _lua_offerchoice);
}

int _lua_wait(lua_State *L) {
	_talk_state *st = (_talk_state*) lua_touserdata(L, lua_upvalueindex(1));
	int ctx, status;
	
	status = lua_getctx(L, &ctx);
	if (status == LUA_YIELD) {
		return 0;
	}
	
	st->m_iState = _WAIT_ANY;
	return lua_yieldk(L, 0, 0, _lua_wait);
}

int _lua_say(lua_State *L) {
	_talk_state *st = (_talk_state*) lua_touserdata(L, lua_upvalueindex(1));
	free(st->m_sNPCText);
	st->m_sNPCText = strdup(lua_tostring(L, 1));
	lua_remove(L, 1);
	return 0;
}

int _lua_stop(lua_State *L) {
	_talk_state *st = (_talk_state*) lua_touserdata(L, lua_upvalueindex(1));
	st->m_iState = _STOPPED;
	return 0;
}
