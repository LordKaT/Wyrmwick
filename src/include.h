#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "config.h"

extern "C" {
	#include "lua-5.2.3/lua.h"
	#include "lua-5.2.3/lualib.h"
	#include "lua-5.2.3/lauxlib.h"
}

#ifdef SCREEN_SDL2
#include "SDL2-2.0.1/SDL.h"

#include "sdl2.h"
#endif

#ifdef AUDIO_SDL2MIXER
#include "SDL2-2.0.1/SDL_mixer.h"
#endif

#include "define.h"
#include "structs.h"
#include "globals.h"
#include "debug.h"
#include "script.h"
#include "screen.h"
#include "font.h"
#include "menu.h"
#include "map.h"
#include "audio.h"

#endif
