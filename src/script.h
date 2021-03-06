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

void script_exec_dir(lua_State *L, const char *path);

// script_remember_thread prevents a Lua thread created in C code from being removed by the GC.
lua_State* script_remember_thread(lua_State *L, lua_State *thread);
void script_forget_thread(lua_State *L, lua_State *thread);

#endif
