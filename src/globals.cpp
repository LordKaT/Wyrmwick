#include "include.h"

lua_State *g_luaState;
item g_items[ITEMS_MAX];
skill g_skills[SKILLS_MAX];
quest g_quests[QUESTS_MAX];
npc g_npc[NPC_MAX];
bool g_bRun;
SDL_Window *g_sdlWindow;
SDL_Renderer *g_sdlRenderer;
SDL_Event g_sdlEvent;
font g_font;
int g_iGameState;
menu g_menu;
