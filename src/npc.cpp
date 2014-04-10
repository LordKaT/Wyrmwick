#include "include.h"

int npc_event(const char *npcname, const char *evhandler) {
	lua_State *thread = lua_newthread(g_luaState);
	script_remember_thread(g_luaState, thread);
	lua_pop(g_luaState, 1);
	
	lua_getglobal(thread, "npc");
	lua_getfield(thread, -1, "list");
	lua_getfield(thread, -1, npcname);
	
	if (lua_isnil(thread, -1)) {
		lua_pushfstring(g_luaState, "NPC \"%s\" doesn't exist", npcname);
		script_forget_thread(g_luaState, thread);
		return LUA_ERRRUN;
	}
	
	lua_getfield(thread, -1, evhandler);
	if (lua_isnil(thread, -1)) {
		debug_print("warning: missing NPC event handler: %s -> %s\n", npcname, evhandler);
		script_forget_thread(g_luaState, thread);
		return 0;
	}
	
	lua_copy(thread, -1, 1);
	lua_copy(thread, -2, 2);
	lua_settop(thread, 2);
	
	int ret = lua_resume(thread, nullptr, 1);
	if (ret == LUA_OK) {
		script_forget_thread(g_luaState, thread);
		return 0;
	}
	if (ret != LUA_YIELD) {
		lua_xmove(thread, g_luaState, 1);
		script_forget_thread(g_luaState, thread);
		return LUA_ERRRUN;
	}
	return 0;
}
