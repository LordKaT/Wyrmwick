#include "include.h"

void script_init() {
	debug_print("Loading Lua 5.2.1 ...\n");
	g_luaState = luaL_newstate();
	luaL_openlibs(g_luaState);
	
	luaL_Reg funcs[] = {
		{ "startTalking", talk_lua_start },
		{ nullptr, nullptr },
	};
	luaL_newlib(g_luaState, funcs);
	lua_setglobal(g_luaState, "game");

	debug_print("	defining Lua globals...\n");
	LUA_DEFINE("EQ_NONE", EQ_NONE);
	LUA_DEFINE("EQ_HEAD", EQ_HEAD);
	LUA_DEFINE("EQ_FACE", EQ_FACE);
	LUA_DEFINE("EQ_NECK", EQ_NECK);
	LUA_DEFINE("EQ_SHOULDERS", EQ_SHOULDERS);
	LUA_DEFINE("EQ_ARMS", EQ_ARMS);
	LUA_DEFINE("EQ_HANDS", EQ_HANDS);
	LUA_DEFINE("EQ_BODY", EQ_BODY);
	LUA_DEFINE("EQ_BACK", EQ_BACK);
	LUA_DEFINE("EQ_WAIST", EQ_WAIST);
	LUA_DEFINE("EQ_LEGS", EQ_LEGS);
	LUA_DEFINE("EQ_WIELD_L", EQ_WIELD_L);
	LUA_DEFINE("EQ_WIELD_R", EQ_WIELD_R);
	LUA_DEFINE("EQ_EAR_L", EQ_EAR_L);
	LUA_DEFINE("EQ_EAR_R", EQ_EAR_R);
	LUA_DEFINE("EQ_RING_1", EQ_RING_1);
	LUA_DEFINE("EQ_RING_2", EQ_RING_2);
	LUA_DEFINE("EQ_ACC_1", EQ_ACC_1);
	LUA_DEFINE("EQ_ACC_2", EQ_ACC_2);
	LUA_DEFINE("EQ_ACC_3", EQ_ACC_3);
	LUA_DEFINE("EQ_ACC_4", EQ_ACC_4);
	LUA_DEFINE("EQ_CRAFT", EQ_CRAFT);
	LUA_DEFINE("EQ_AMMO", EQ_AMMO);
	LUA_DEFINE("EQ_MAX", EQ_MAX);

	LUA_DEFINE("ITEM_NONE", ITEM_NONE);
	LUA_DEFINE("ITEM_EQUIP", ITEM_EQUIP);
	LUA_DEFINE("ITEM_DRINK", ITEM_DRINK);
	LUA_DEFINE("ITEM_FOOD", ITEM_FOOD);
	LUA_DEFINE("ITEM_MATERIAL", ITEM_MATERIAL);
	LUA_DEFINE("ITEM_MONEY", ITEM_MONEY);
	LUA_DEFINE("ITEM_SPELL", ITEM_SPELL);
	LUA_DEFINE("ITEM_SCROLL", ITEM_SCROLL);
	LUA_DEFINE("ITEM_PEN", ITEM_PEN);
	LUA_DEFINE("ITEM_STORAGE", ITEM_STORAGE);

	LUA_DEFINE("WEAPON_NONE", WEAPON_NONE);
	LUA_DEFINE("WEAPON_DAGGER", WEAPON_DAGGER);
	LUA_DEFINE("WEAPON_SWORD_SHORT", WEAPON_SWORD_SHORT);
	LUA_DEFINE("WEAPON_SWORD_LONG", WEAPON_SWORD_LONG);
	LUA_DEFINE("WEAPON_SWORD_TWOHAND", WEAPON_SWORD_TWOHAND);
	LUA_DEFINE("WEAPON_CLUB", WEAPON_CLUB);
	LUA_DEFINE("WEAPON_CLUB_TWOHAND", WEAPON_CLUB_TWOHAND);
	LUA_DEFINE("WEAPON_STAFF", WEAPON_STAFF);
	LUA_DEFINE("WEAPON_ROD", WEAPON_ROD);
	LUA_DEFINE("WEAPON_CLAW", WEAPON_CLAW);
	LUA_DEFINE("WEAPON_BOW_SHORT", WEAPON_BOW_SHORT);
	LUA_DEFINE("WEAPON_BOW_LONG", WEAPON_BOW_LONG);
	LUA_DEFINE("WEAPON_CROSSBOW", WEAPON_CROSSBOW);
	LUA_DEFINE("WEAPON_THROW", WEAPON_THROW);

	LUA_DEFINE("STATS_HP", STATS_HP);
	LUA_DEFINE("STATS_MAX_HP", STATS_MAX_HP);
	LUA_DEFINE("STATS_SP", STATS_SP);
	LUA_DEFINE("STATS_MAX_SP", STATS_MAX_SP);
	LUA_DEFINE("STATS_STR", STATS_STR);
	LUA_DEFINE("STATS_DEX", STATS_DEX);
	LUA_DEFINE("STATS_AGL", STATS_AGL);
	LUA_DEFINE("STATS_WIS", STATS_WIS);
	LUA_DEFINE("STATS_INT", STATS_INT);
	LUA_DEFINE("STATS_ATK", STATS_ATK);
	LUA_DEFINE("STATS_DEF", STATS_DEF);

	LUA_DEFINE("STATS_MAX", STATS_MAX);

	LUA_DEFINE("INV_MAX", INV_MAX);
	LUA_DEFINE("SKILLS_MAX", SKILLS_MAX);
	LUA_DEFINE("STATUS_MAX", STATUS_MAX);
	LUA_DEFINE("ITEMS_MAX", ITEMS_MAX);
	
	debug_print("\tloading utility library...\n");
	script_exec_dir(g_luaState, "data/lib");
	
	debug_print("\tloading NPCs...\n");
	script_exec_dir(g_luaState, "data/npc");

	// FIXME: We never free these.
	debug_print("	loading item scripts...\n");
	script_load_items();
	debug_print("	loading skill scripts...\n");
	script_load_skills();
	debug_print("	loading quest scripts...\n");
	script_load_quests();
	debug_print("	Lua loaded!\n");
	return;
}

void script_run_chunk(void *vChunk, lua_Writer writer, lua_Reader reader) {
	lua_load(g_luaState, reader, vChunk, "chunk", nullptr);
	lua_pcall(g_luaState, 0, 0, 0);
	return;
}

void script_call_function(const char *cFunc, int iArgs, int iRes, int iErr) {
	lua_getglobal(g_luaState, cFunc);
	lua_pcall(g_luaState, iArgs, iRes, iErr);
	return;
}

int script_luaWriter(lua_State *luaState, const void *p, size_t size, void *vData) {
	luaChunk *pChunk = (luaChunk *)vData;
	void *vAlloc = realloc(pChunk->m_vLuaChunk, pChunk->m_sizeLuaChunk + size);
	if (vAlloc == 0 || vAlloc == nullptr) {
		printf("Error: out of memory.\n");
	} else {
		pChunk->m_vLuaChunk = (char *)vAlloc;
	}
	memcpy((char *)pChunk->m_vLuaChunk + pChunk->m_sizeLuaChunk, p, size);
	pChunk->m_sizeLuaChunk += size;
	return 0;
}

const char *script_luaReader(lua_State *luaState, void *vData, size_t *size) {
	luaChunk *pChunk = (luaChunk *)vData;
	size = &pChunk->m_sizeLuaChunk;
	return (char *)pChunk->m_vLuaChunk;
}

void script_load_items() {
	FILE *file =  fopen("data/items/items_list.txt", "r");
	if (file != nullptr) {
		char cLine[512];
		char cFile[512];
		int iItemID = 0;
		memset(&cLine, 0, sizeof(cLine));
		memset(&cFile, 0, sizeof(cFile));
		while (fgets(cLine, sizeof(cLine), file) != nullptr) {
			strtok(cLine, "\n");
			strcpy(cFile, "data/items/scripts/");
			strcat(cFile, cLine);
			strcat(cFile, ".lua");
			g_items[iItemID].m_cLuaScript = (char *)malloc(sizeof(cFile)+1);
			strcpy(g_items[iItemID].m_cLuaScript, cFile);
			int iRes = luaL_loadfile(g_luaState, cFile);
			if (iRes == 0) { /* good. */
				lua_dump(g_luaState, &script_luaWriter, &g_items[iItemID].m_luaChunk);
				script_run_chunk(&g_items[iItemID].m_luaChunk.m_vLuaChunk, &script_luaWriter, script_luaReader);
				script_call_function("item_init", 0, 5, 0);

				if (!lua_isstring(g_luaState, -5)) {
					printf("error: init() must return: (string), string, int, int, int.\n");
				} else {
					size_t sTemp;
					const char *cTemp = lua_tolstring(g_luaState, -5, &sTemp);
					g_items[iItemID].m_cName = (char *)malloc(sTemp + 1);
					memcpy(g_items[iItemID].m_cName, cTemp, sTemp + 1);
				}

				if (!lua_isstring(g_luaState, -4)) {
					printf("error: init() must return: string, (string), int, int, int.\n");
				} else {
					size_t sTemp;
					const char *cTemp = lua_tolstring(g_luaState, -4, &sTemp);
					g_items[iItemID].m_cDesc = (char *)malloc(sTemp + 1);
					memcpy(g_items[iItemID].m_cDesc, cTemp, sTemp + 1);
				}

				if (!lua_isnumber(g_luaState, -3)) {
					printf("error: init() must return: string, string, (int), int, int.\n");
				} else {
					g_items[iItemID].m_iItemType = lua_tointeger(g_luaState, -3);
				}

				if (!lua_isnumber(g_luaState, -2)) {
					printf("error: init() must return: string, string, int, (int), int.\n");
				} else {
					g_items[iItemID].m_iWeaponType = lua_tointeger(g_luaState, -2);
				}

				if (!lua_isnumber(g_luaState, -1)) {
					printf("error: init() must return: string, string, int, int, (int).\n");
				} else {
					g_items[iItemID].m_iEquipType = lua_tointeger(g_luaState, -1);
				}

				lua_pop(g_luaState, -1);

			} else {
				printf("script_load_items(): failed to load %s\n", cFile);
				printf("%s\n", luaL_checkstring(g_luaState, -1));
			}
			memset(&cLine, 0, sizeof(cLine));
			memset(&cFile, 0, sizeof(cFile));
			iItemID++;
		}
		fclose(file);
	} else {
		printf("Error: cannot load items_list.txt\n");
		return;
	}
	return;
}

void script_load_skills() {
	FILE *file =  fopen("data/skills/skills_list.txt", "r");
	if (file != nullptr) {
		char cLine[512];
		char cFile[512];
		int iSkillID = 0;
		memset(&cLine, 0, sizeof(cLine));
		memset(&cFile, 0, sizeof(cFile));
		while (fgets(cLine, sizeof(cLine), file) != nullptr) {
			strtok(cLine, "\n");
			strcpy(cFile, "data/skills/scripts/");
			strcat(cFile, cLine);
			strcat(cFile, ".lua");
			g_skills[iSkillID].m_cLuaScript = (char *)malloc(sizeof(cFile)+1);
			strcpy(g_skills[iSkillID].m_cLuaScript, cFile);
			int iRes = luaL_loadfile(g_luaState, cFile);
			if (iRes == 0) { /* good. */
				lua_dump(g_luaState, &script_luaWriter, &g_skills[iSkillID].m_luaChunk);
				script_run_chunk(&g_skills[iSkillID].m_luaChunk.m_vLuaChunk, &script_luaWriter, script_luaReader);
				script_call_function("skill_init", 0, 2, 0);

				if (!lua_isstring(g_luaState, -2)) {
					printf("error: skill_init() must return: (string), string.\n");
				} else {
					size_t sTemp;
					const char *cTemp = lua_tolstring(g_luaState, -2, &sTemp);
					g_skills[iSkillID].m_cName = (char *)malloc(sTemp + 1);
					memcpy(g_skills[iSkillID].m_cName, cTemp, sTemp + 1);
				}

				if (!lua_isstring(g_luaState, -1)) {
					printf("error: skill_init() must return: string, (string).\n");
				} else {
					size_t sTemp;
					const char *cTemp = lua_tolstring(g_luaState, -1, &sTemp);
					g_skills[iSkillID].m_cDesc = (char *)malloc(sTemp + 1);
					memcpy(g_skills[iSkillID].m_cDesc, cTemp, sTemp + 1);
				}

				lua_pop(g_luaState, -1);

			} else {
				printf("script_load_skills(): failed to load %s\n", cFile);
				printf("%s\n", luaL_checkstring(g_luaState, -1));
			}
			memset(&cLine, 0, sizeof(cLine));
			memset(&cFile, 0, sizeof(cFile));
			iSkillID++;
		}
		fclose(file);
	} else {
		printf("Error: cannot load skills_list.txt\n");
		return;
	}
	return;
}

void script_load_quests() {
	FILE *file =  fopen("data/quests/quests_list.txt", "r");
	if (file != nullptr) {
		char cLine[512];
		char cFile[512];
		int iQuestID = 0;
		memset(&cLine, 0, sizeof(cLine));
		memset(&cFile, 0, sizeof(cFile));
		while (fgets(cLine, sizeof(cLine), file) != nullptr) {
			strtok(cLine, "\n");
			strcpy(cFile, "data/quests/scripts/");
			strcat(cFile, cLine);
			strcat(cFile, ".lua");
			g_quests[iQuestID].m_cLuaScript = (char *)malloc(sizeof(cFile)+1);
			strcpy(g_quests[iQuestID].m_cLuaScript, cFile);
			int iRes = luaL_loadfile(g_luaState, cFile);
			if (iRes == 0) { /* good. */
				lua_dump(g_luaState, &script_luaWriter, &g_quests[iQuestID].m_luaChunk);
				script_run_chunk(&g_quests[iQuestID].m_luaChunk.m_vLuaChunk, &script_luaWriter, script_luaReader);
				script_call_function("quest_init", 0, 2, 0);

				if (!lua_isstring(g_luaState, -2)) {
					printf("error: quest_init() must return: (string), string.\n");
				} else {
					size_t sTemp;
					const char *cTemp = lua_tolstring(g_luaState, -2, &sTemp);
					g_quests[iQuestID].m_cName = (char *)malloc(sTemp);
					memset(g_quests[iQuestID].m_cName, 0, sTemp);
					memcpy(g_quests[iQuestID].m_cName, cTemp, sTemp + 1);
				}

				if (!lua_isstring(g_luaState, -1)) {
					printf("error: quest_init() must return: string, (string).\n");
				} else {
					size_t sTemp;
					const char *cTemp = lua_tolstring(g_luaState, -1, &sTemp);
					g_quests[iQuestID].m_cDesc = (char *)malloc(sTemp);
					memset(g_quests[iQuestID].m_cDesc, 0, sTemp);
					memcpy(g_quests[iQuestID].m_cDesc, cTemp, sTemp + 1);
				}

				lua_pop(g_luaState, -1);

			} else {
				printf("script_load_skills(): failed to load %s\n", cFile);
				printf("%s\n", luaL_checkstring(g_luaState, -1));
			}
			memset(&cLine, 0, sizeof(cLine));
			memset(&cFile, 0, sizeof(cFile));
			iQuestID++;
		}
		fclose(file);
	} else {
		printf("Error: cannot load skills_list.txt\n");
		return;
	}
	return;
}


void script_exec_dir(lua_State *L, const char *path) {
	sys_dir *dir;
	char pathbuff[1024];
	int pathlen;
	
	pathlen = strlen(path);
	strcpy(pathbuff, path);
	strcat(pathbuff, "/");
	
	dir = sys_dir_open(path);
	if (!dir) {
		debug_print("script_exec_dir: Failed to open %s\n", path);
		return;
	}
	
	const char *fpath;
	while ((fpath = sys_dir_next(dir)) != nullptr) {
		if (fpath[0] == '.') { continue; }
		if (strlen(fpath) < 4 || strcmp(fpath + strlen(fpath)-4, ".lua") != 0) {
			debug_print("script_exec_dir: ignoring '%s': invalid extension\n", fpath);
			continue;
		}
		
		strcpy(pathbuff + pathlen + 1, fpath);
		bool err = luaL_dofile(L, pathbuff);
		if (err) {
			const char* errmsg = lua_tostring(L, -1);
			debug_print("%s\n", errmsg);
		}
	}
	sys_dir_close(dir);
}

lua_State* script_remember_thread(lua_State *L, lua_State *thread) {
	lua_getfield(L, LUA_REGISTRYINDEX, "script/threads");
	if (lua_isnil(L, -1)) {
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setfield(L, LUA_REGISTRYINDEX, "script/threads");
	}

	lua_pushthread(thread);
	lua_xmove(thread, L, 1);
	lua_pushboolean(L, true);
	lua_settable(L, -3);
	lua_pop(L, 1);
	return thread;
}

void script_forget_thread(lua_State *L, lua_State *thread) {
	lua_getfield(L, LUA_REGISTRYINDEX, "script/threads");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		debug_print("warning: script_forget_thread: script/threads not initialized\n");
		return;
	}
	
	lua_pushthread(thread);
	lua_xmove(thread, L, 1);
	lua_pushvalue(L, -1);
	lua_gettable(L, -3);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		debug_print("warning: script_forget_thread: thankfully I already don't remember this thread\n");
		return;
	}
	
	lua_pop(L, 1);
	lua_pushnil(L);
	lua_settable(L, LUA_REGISTRYINDEX);
	lua_pop(L, 1);
}
