#include "include.h"

#define SCREEN_SDL2

static int _lua_screen_resolution(lua_State *L) {
	if (lua_gettop(L) != 2) {
		lua_pushstring(L, "function screen_resolution needs exactly 2 arguments");
		lua_error(L);
		return 0;
	}
	
	if ((! lua_isnumber(L, 1)) || (! lua_isnumber(L, 2))) {
		lua_pushstring(L, "arguments of function screen_resolution must be (number, number)");
		lua_error(L);
		return 0;
	}
	
	g_iScreenWidth = lua_tointeger(L, 1);
	g_iScreenHeight = lua_tointeger(L, 2);
	return 0;
}

static int _write_settings(FILE* file, void* udata) {
	fprintf(file, "screen_resolution(%d, %d)\n", g_iScreenWidth, g_iScreenHeight);
	return 0;
}

void screen_config_settings(settings* st, void* udata) {
	settings_add_func(st, "screen_resolution", &_lua_screen_resolution, nullptr);
	settings_add_writer(st, &_write_settings, nullptr);
}

void screen_init() {
	debug_print("Loading Screen ...\r\n");
#ifdef SCREEN_SDL2
	debug_print("	screen: SDL2\r\n");
	sdl2_init();
#endif
	return;
}

void screen_clear() {
#ifdef SCREEN_SDL2
	SDL_RenderClear(g_sdlRenderer);
#endif
	return;
}

void screen_present() {
#ifdef SCREEN_SDL2
	SDL_RenderPresent(g_sdlRenderer);
#endif
	return;
}

void screen_destroy() {
#ifdef SCREEN_SDL2
	sdl2_destroy();
#endif
	return;
}
