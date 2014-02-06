#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include "include.h"

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

struct image {
#ifdef SCREEN_SDL2
	SDL_Texture *m_image;
#endif
};

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
	image m_image;
	rect m_rectClip[256];
	rect m_rectDraw[256];
	char *m_cText;
};

struct menu {
	char *m_cLabel;
	int m_iCursorPos;
	unsigned int m_uiMenuWidth;
	unsigned int m_uiMenuHeight;
	char *m_cMenuItem[16];
	void (*m_vFunc)(int iSelection);
	bool m_bIsOpen;
	char *m_cLuaScript;
	luaChunk m_luaChunk;
	image m_image;
};

struct map {
	char *m_cName;
	rect m_rect[100][100];
	rect m_rectView;
	rect m_rectDest;
	image m_imageMap;
	image m_imageTiles;
};

#endif
