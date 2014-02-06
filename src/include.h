#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

// dependency includes need to be like this
// because the cmake scripts point to the actual include directories

#include "SDL.h"

// Define nullptr; not part of GCC norm, I guess it's a VC++ism?
#ifndef nullptr
#define nullptr (0)
#endif

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "define.h"
#include "structs.h"
#include "globals.h"
#include "debug.h"
#include "script.h"
#include "screen.h"
#include "sdl_helpers.h" // this had to be renamed because it conflicted with the real SDL.h
#include "font.h"
#include "menu.h"

#endif
