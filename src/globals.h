#ifndef __GLOBALS_H__
#define __GLOBALS_H__

extern lua_State *g_luaState;
extern item g_items[ITEMS_MAX];
extern skill g_skills[SKILLS_MAX];
extern quest g_quests[QUESTS_MAX];
extern npc g_npc[NPC_MAX];
extern bool g_bRun;
extern font g_font;
extern int g_iGameState;
extern menu g_menu;

extern SDL_Window *g_sdlWindow;
extern SDL_Renderer *g_sdlRenderer;
extern SDL_Event g_sdlEvent;

extern SDL_Texture *g_sdlMapTex;
extern map g_map;
#endif
