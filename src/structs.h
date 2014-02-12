#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include "include.h"

struct array {
	void* m_data;
	size_t m_elemSize;
	int m_len, m_cap;
};

struct luaChunk {
	void *m_vLuaChunk;
	size_t m_sizeLuaChunk;
};

#ifdef SCREEN_SDL2
typedef SDL_Rect rect;
#else
struct rect {
	int x;
	int y;
	int w;
	int h;
};
#endif

#ifdef SCREEN_SDL2
typedef SDL_Texture image;
#else
struct image {};
#endif

struct audio {
	int *m_iPosition;
	int m_iLength;
#ifdef AUDIO_SDL2MIXER
	Mix_Chunk *m_sound;
	Mix_Music *m_music;
#endif
};

struct item {
	char *m_cName;
	char *m_cDesc;
	int m_iItemType;
	int m_iWeaponType;
	int m_iEquipType;
	char *m_cLuaScript;
	luaChunk m_luaChunk;
};

struct itemList {
	item m_item;
	itemList *next;
};

struct skill {
	char *m_cName;
	char *m_cDesc;
	char *m_cLuaScript;
	luaChunk m_luaChunk;
};

struct inventory {
	item *m_pItem;
	int m_iQuantity;
};

struct quest {
	char *m_cName;
	char *m_cDesc;
	char *m_cLuaScript;
	int m_iQuestFlags[FLAGS_MAX];
	luaChunk m_luaChunk;
};

struct npc {
	char *m_cName;
	char *m_cDesc;
	int m_iStats[STATS_MAX];
	item *m_pEquipment[EQ_MAX];
	inventory m_iInventory[INV_MAX];
	char *m_cLuaScript;
	luaChunk m_luaChunk;
};

struct smartObject {
	char m_cName[256];
	int m_iStats[STATS_MAX];
	item *m_pEquipment[EQ_MAX];
	inventory m_iInventory[INV_MAX];
	skill m_sSkill[SKILLS_MAX];
	quest m_qQuests[QUESTS_MAX];
};

struct font {
	image *m_image;
	int m_iGlyphWidth, m_iGlyphHeight;
};

struct menu {
	char *m_cLabel;
	char *m_cInput;
	int m_iCursorPos;
	unsigned int m_uiMenuWidth;
	unsigned int m_uiMenuHeight;
	char *m_cMenuItem[16];
	void (*m_vFunc)(int iSelection);
	bool m_bIsOpen;
	char *m_cLuaScript;
	luaChunk m_luaChunk;
	image *m_image;
};

struct mapEditor {
	int m_iActiveTile;
	int m_iMouseX;
	int m_iMouseY;
	int m_iMapEditorState;
	char m_cMapWalk;
	bool m_bDragMap;
	bool m_bGrid;
};

struct mapData {
	int m_iTileID;		// Id of sprite
	char m_cWalk;		// Walkability of this tile (non, climb, walk, fly, etc...)
};

struct map {
	/* This data is saved to file. */
	char *m_cName;
	char *m_cTileset;
	char *m_cLuaScript;
	mapData m_map[MAP_SIZE][MAP_SIZE];

	/* Not saved to file. */
	rect m_rectView;
	rect m_rectDest;
	image *m_imageMap;	// Map is drawn to this texture
	image *m_imageTiles;	// Tile image storage
	luaChunk m_luaChunk;
};

struct input_state {
	int m_iDirection;
	int m_iActive;
};

struct input_event {
	int m_iType;  // IN_NONE, IN_ON, IN_OFF or IN_DIRCHANGE
	int m_iKey;  // key if ON/OFF or new direction for DIRCHANGE
};

struct input_control {
	int m_type;             // IN_TYPE_KEYBOARD, IN_TYPE_JOYBUTTON or IN_TYPE_JOYAXIS
	Uint8 m_iIndex;         // joystick button/axis index
	int m_iAxisDir;       // axis direction, 1 or -1
	SDL_Keycode m_keycode;
	
	int m_iTo;    // logical button (IN_DIRUP etc.)
};

typedef int (*settings_writer_func)(FILE *sfile, void *userdata);

struct settings_writer {
	settings_writer_func m_func;
	void *m_userdata;
};

struct settings {
	lua_State *m_luaState;
	array /*of settings_writer*/ *m_aWriters;
};

#endif
