#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "SDL2-2.0.1/SDL.h"

// Define nullptr; not part of GCC norm, I guess it's a VC++ism?
#ifndef nullptr
#define nullptr (0)
#endif

extern "C" {
	#include "lua-5.2.3/lua.h"
	#include "lua-5.2.3/lualib.h"
	#include "lua-5.2.3/lauxlib.h"
}

#include "define.h"
#include "structs.h"
#include "globals.h"
#include "debug.h"
#include "script.h"
#include "screen.h"
#include "sdl.h"
#include "font.h"
#include "menu.h"

#endif
