#include "include.h"

text_entry *text_entry_init(int x, int y) {
	text_entry *te = (text_entry*) malloc(sizeof(text_entry));
	te->m_iX = x;
	te->m_iY = y;
	te->m_iPos = te->m_iLen = 0;
	te->m_iCap = 64;
	
	te->m_sBuff = (char*) malloc(te->m_iCap);
	return te;
}

void _mcursor(text_entry *te, int arg) {
	if (arg > 0) {
		if (te->m_iPos == te->m_iLen) { return; }
		++te->m_iPos;
		return;
	}
	if (te->m_iPos == 0) { return; }
	--te->m_iPos;
}

void _scursor(text_entry *te, int arg) {
	if (arg == 0) {
		te->m_iPos = 0;
		return;
	}
	te->m_iPos = te->m_iLen;
}

void _delchar(text_entry *te, int arg) {
	(void) arg;
	if (te->m_iPos == 0) { return; }
	memmove(te->m_sBuff+te->m_iPos-1, te->m_sBuff+te->m_iPos, te->m_iLen-te->m_iPos);
	--te->m_iPos;
	--te->m_iLen;
}

void _delline(text_entry *te, int arg) {
	(void) arg;
	memmove(te->m_sBuff, te->m_sBuff+te->m_iPos, te->m_iLen-te->m_iPos);
	te->m_iLen -= te->m_iPos;
	te->m_iPos = 0;
}

void _copy(text_entry *te, int arg) {
	(void) arg;
	SDL_SetClipboardText(te->m_sBuff);
}

void _paste(text_entry *te, int arg) {
	(void) arg;
	if (! SDL_HasClipboardText()) { return; }
	
	const char *txt = SDL_GetClipboardText();
	if (! txt) {
		debug_print("Failed to paste: %s\n", SDL_GetError());
		return;
	}
	
	int len = strlen(txt);
	bool newmem = false;
	while (te->m_iLen + len > te->m_iCap) {
		te->m_iCap *= 2;
		newmem = true;
	}
	if (newmem) {
		te->m_sBuff = (char*) realloc(te->m_sBuff, te->m_iCap);
		if (! te->m_sBuff) {
			debug_print("Out of memory!\n");
			sys_abort();
		}
	}
	memmove(te->m_sBuff+te->m_iPos+len, te->m_sBuff+te->m_iPos, te->m_iLen-te->m_iPos);
	memmove(te->m_sBuff+te->m_iPos, txt, len);
	te->m_iLen += len;
	te->m_iPos += len;
}
	
bool text_entry_input(text_entry *te, SDL_Event *sdlEvent) {
	struct {SDL_Keycode key; Uint16 mod; void (*func)(text_entry*,int); int arg;
	} keycmd[] = {
		{SDLK_LEFT,	KMOD_NONE,	&_mcursor,	-1},
		{SDLK_RIGHT,	KMOD_NONE,	&_mcursor,	1},
		
		{SDLK_a,	KMOD_CTRL,	&_scursor,	0},
		{SDLK_e,	KMOD_CTRL,	&_scursor,	1},
		{SDLK_h,	KMOD_CTRL,	&_delchar,	0},
		{SDLK_u,	KMOD_CTRL,	&_delline,	0},
		
		{SDLK_BACKSPACE,	KMOD_NONE,	&_delchar,	0},
		
		{SDLK_c,	KMOD_CTRL,	&_copy,	0},
		{SDLK_v,	KMOD_CTRL,	&_paste,	0},
	};
	
	if (sdlEvent->type == SDL_KEYDOWN) {
		for (int i = 0; i < SIZE(keycmd); i++) {
			if (sdlEvent->key.keysym.sym != keycmd[i].key) { continue; }
			if (keycmd[i].mod != KMOD_NONE) {
				if ((sdlEvent->key.keysym.mod & keycmd[i].mod) == 0) { continue; }
			}
			keycmd[i].func(te, keycmd[i].arg);
			return true;
		}
		return false;
	}
	
	if (sdlEvent->type != SDL_TEXTINPUT) { return false; }
	for (char *ptr = sdlEvent->text.text; *ptr != '\0'; ptr++) {
		if (te->m_iLen == te->m_iCap) {
			te->m_iCap *= 2;
			te->m_sBuff = (char*) realloc(te->m_sBuff, te->m_iCap);
		}
		memmove(te->m_sBuff+te->m_iPos+1, te->m_sBuff+te->m_iPos, te->m_iLen-te->m_iPos);
		te->m_sBuff[te->m_iPos] = *ptr;
		++te->m_iPos;
		++te->m_iLen;
	}
	return true;
}

const char* text_entry_text(text_entry *te) {
	if (te->m_iLen == te->m_iCap) {
		te->m_iCap *= 2;
		te->m_sBuff = (char*) realloc(te->m_sBuff, te->m_iCap);
		if (! te->m_sBuff) {
			debug_print("Out of memory!\n");
			sys_abort();
		}
	}
	te->m_sBuff[te->m_iLen] = '\0';
	return te->m_sBuff;
}

int text_entry_render(text_entry *te) {
	char txt[256];
	int tlen;
	const char *alltxt = text_entry_text(te);
	image_setcolormod(g_font.m_image, COLOR_MENU_TEXT);
	tlen = font_print(g_font, te->m_iX, te->m_iY, "%s", alltxt);
	
	if (strlen(alltxt) > 255) { return tlen; }
	memmove(txt, te->m_sBuff, te->m_iPos);
	txt[te->m_iPos] = '\0';
	
	int x = te->m_iX + font_text_width(g_font, txt);
	int y = te->m_iY;
	rect cursor = {x, y, 3, g_font.m_iGlyphHeight};
	screen_fill_rect(&cursor, COLOR_MENU_TEXT);
	
	return tlen;
}

void text_entry_destroy(text_entry *te) {
	free(te->m_sBuff);
	free(te);
}
