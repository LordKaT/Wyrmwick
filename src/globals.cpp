#include "include.h"

bool g_bRun;
int g_iGameState;
lua_State *g_luaState;
item g_items[ITEMS_MAX];
skill g_skills[SKILLS_MAX];
quest g_quests[QUESTS_MAX];
npc g_npc[NPC_MAX];
font g_font;
menu g_menu;
map g_map;
audio g_audio;

#ifdef SCREEN_SDL2
SDL_Window *g_sdlWindow;
SDL_Renderer *g_sdlRenderer;
SDL_Event g_sdlEvent;
#endif
