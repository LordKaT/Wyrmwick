#ifndef __GLOBALS_H__
#define __GLOBALS_H__

extern bool g_bRun;
extern int g_iGameState;
extern lua_State *g_luaState;
extern item g_items[ITEMS_MAX];
extern skill g_skills[SKILLS_MAX];
extern quest g_quests[QUESTS_MAX];
extern npc g_npc[NPC_MAX];
extern font g_font;
extern menu g_menu;
extern map g_map;
extern audio g_audio;

#ifdef SCREEN_SDL2
extern SDL_Window *g_sdlWindow;
extern SDL_Renderer *g_sdlRenderer;
extern SDL_Event g_sdlEvent;
#endif

#endif
