#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include "config.h"

extern "C" {
	#include "lua-5.2.3/lua.h"
	#include "lua-5.2.3/lualib.h"
	#include "lua-5.2.3/lauxlib.h"
}

#ifdef SCREEN_SDL2
#include "SDL2-2.0.1/SDL.h"
#include "SDL2-2.0.1/SDL_image.h"
#include "screen_sdl2.h"
#endif

#ifdef AUDIO_SDL2MIXER
#include "SDL2-2.0.1/SDL_mixer.h"
#include "audio_sdl2mixer.h"
#endif

#include "define.h"
#include "structs.h"

#include "audio.h"
#include "debug.h"
#include "file_dialog.h"
#include "font.h"
#include "globals.h"
#include "image.h"
#include "animator.h"
#include "input.h"
#include "main_menu.h"
#include "map.h"
#include "map_editor.h"
#include "menu.h"
#include "screen.h"
#include "script.h"
#include "settings.h"
#include "settings_menu.h"
#include "sys.h"
#include "tables.h"
#include "text_entry.h"
#include "utils.h"

#endif
