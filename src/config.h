#define SCREEN_SDL2
/*
	Future options?

	#define SCREEN_OPENGL
	#define SCREEN_SDL1
*/

#define AUDIO_SDL2MIXER
/*
	Future options?

	#define AUDIO_SDL2
	#define AUDIO_SDL1
	#define AUDIO_SDL1MIXER
	#define AUDIO_OPENAL
*/

#define MAP_SIZE			128
#define MAP_TEXTURE_SIZE	MAP_SIZE * 32	// 32 = tile size

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720

#define COLOR_MENU_BACK     0x003380ff
#define COLOR_MENU_TEXT     0xffffffff
#define COLOR_MENU_DISABLED 0x636363ff
#define COLOR_MENU_SELECTED 0x5f8dd3ff
#define COLOR_MENU_BORDER   0xb8fffbff