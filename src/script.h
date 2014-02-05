#ifndef __SCRIPT_H__
#define __SCRIPT_H__

void script_init();
void script_run_chunk(void *vChunk, lua_Writer writer, lua_Reader reader);
void script_call_function(char *cFunc, int iArgs, int iRes, int iErr);
int script_luaWriter(lua_State *luaState, const void *p, size_t size, void *vData);
const char *script_luaReader(lua_State *luaState, void *vData, size_t *size);
void script_load_items();
void script_load_skills();
void script_load_quests();
void script_load_npc();

#endif
