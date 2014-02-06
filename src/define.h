#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WYRMWICK_VERSION	"FEB 05 2014 Pre-Alpha"

#define DEBUG	/* Well, yeah. Don't forget to change build type
					if we remove this. */

#define GAME_START	0
#define GAME_DEBUG	1
#define GAME_MENU	2
#define GAME_WORLD	3
#define GAME_SHOP	4
#define GAME_DIALOG	5
#define GAME_BATTLE	6
#define GAME_END	7

#define MENU_NONE	0
#define MENU_TEXT	1
#define MENU_NUMBER	2
#define MENU_BOOL	3


/* Equipment macros. */
#define EQ_NONE			-1
#define EQ_HEAD			0
#define EQ_FACE			1
#define EQ_NECK			2
#define EQ_SHOULDERS	3
#define EQ_ARMS			4
#define EQ_HANDS		5
#define EQ_BODY			6
#define EQ_BACK			7
#define EQ_WAIST		8
#define	EQ_LEGS			9
#define EQ_FEET			10
#define EQ_WIELD_L		11
#define EQ_WIELD_R		12
#define EQ_EAR_L		13
#define EQ_EAR_R		14
#define EQ_RING_1		15
#define EQ_RING_2		16
#define EQ_ACC_1		17
#define EQ_ACC_2		18
#define EQ_ACC_3		19
#define EQ_ACC_4		20
#define EQ_CRAFT		21
#define EQ_AMMO			22

#define EQ_MAX			32


/* Item types. */
#define ITEM_NONE			0
#define ITEM_EQUIP			1
#define ITEM_DRINK			2
#define ITEM_FOOD			3
#define ITEM_MATERIAL		4
#define ITEM_MONEY			5
#define ITEM_SPELL			6
#define ITEM_SCROLL			7
#define ITEM_PEN			8
#define ITEM_STORAGE		9


/* Weapon types. */
#define WEAPON_NONE					0
#define WEAPON_DAGGER				1
#define WEAPON_SWORD_SHORT			2
#define WEAPON_SWORD_LONG			3
#define WEAPON_SWORD_TWOHAND		4
#define WEAPON_CLUB					5
#define WEAPON_CLUB_TWOHAND			6
#define WEAPON_STAFF				7
#define WEAPON_ROD					8
#define WEAPON_CLAW					9
#define WEAPON_BOW_SHORT			10
#define WEAPON_BOW_LONG				11
#define WEAPON_CROSSBOW				12
#define WEAPON_THROW				13

/* Stats. */
#define STATS_HP		0
#define STATS_MAX_HP	1
#define STATS_SP		2
#define STATS_MAX_SP	3
#define STATS_STR		4
#define STATS_DEX		5
#define STATS_AGL		6
#define STATS_WIS		7
#define STATS_INT		8
#define STATS_ATK		9
#define STATS_DEF		10

#define STATS_MAX		512

#define INV_MAX		1024
#define SKILLS_MAX	1024
#define STATUS_MAX	1024
#define ITEMS_MAX	1024
#define FLAGS_MAX	32
#define QUESTS_MAX	1024
#define NPC_MAX		1024


#define LUA_DEFINE(c, i)	lua_pushnumber(g_luaState, i); \
							lua_setglobal(g_luaState, c);	

#define LUA_VARIABLE_STRING(g, c, s)	lua_pushlstring(g, s, strlen(s)); \
										lua_setglobal(g, c);

#define LUA_VARIABLE_INT(g, c, i)		lua_pushnumber(g, i); \
										lua_setglobal(g, c);

#define LUA_FUNCTION(g, c, f)	lua_pushcfunction(g, f); \
								lua_setglobal(g, c);

#endif
