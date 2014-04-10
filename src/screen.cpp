#include "include.h"

#define SCREEN_SDL2

static int _lua_screen_resolution(lua_State *L) {
	if (lua_gettop(L) != 2) {
		return luaL_error(L, "function screen_resolution needs exactly 2 arguments");
	}
	
	if ((! lua_isnumber(L, 1)) || (! lua_isnumber(L, 2))) {
		return luaL_error(L, "arguments of function screen_resolution must be (number, number)");
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
	debug_print("Loading Screen ...\n");
#ifdef SCREEN_SDL2
	debug_print("	screen: SDL2\n");
	sdl2_init();
#endif
	return;
}

void screen_fill_rect(rect *dst, Uint32 color) {
#ifdef SCREEN_SDL2
	SDL_SetRenderDrawColor(g_sdlRenderer, (color&0xff000000)>>24, (color&0xff0000)>>16, (color&0xff00)>>8, color&0xff);
	SDL_RenderFillRect(g_sdlRenderer, dst);
#endif
}

void screen_draw_rect(rect *dst, Uint32 color) {
#ifdef SCREEN_SDL2
	SDL_SetRenderDrawColor(g_sdlRenderer, (color&0xff000000)>>24, (color&0xff0000)>>16, (color&0xff00)>>8, color&0xff);
	SDL_RenderDrawRect(g_sdlRenderer, dst);
#endif
}

void screen_draw_line(int x1, int y1, int x2, int y2, Uint32 color) {
	SDL_SetRenderDrawColor(g_sdlRenderer, (color&0xff000000)>>24, (color&0xff0000)>>16, (color&0xff00)>>8, color&0xff);
	SDL_RenderDrawLine(g_sdlRenderer, x1, y1, x2, y2);
}

void screen_clear() {
#ifdef SCREEN_SDL2
	SDL_SetRenderDrawColor(g_sdlRenderer, 0, 0, 0, 255);
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
