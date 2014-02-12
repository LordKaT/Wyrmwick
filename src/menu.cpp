#include "include.h"

void menu_init() {
	/* this is wrong. */
	debug_print("Loading Menu ...\r\n");
	if (g_menu.m_cLabel != nullptr) {
		free(g_menu.m_cLabel);
		g_menu.m_cLabel = nullptr;
	}
	if (g_menu.m_cLuaScript != nullptr) {
		free(g_menu.m_cLuaScript);
		g_menu.m_cLuaScript = nullptr;
	}
	for (int i = 0; i < 16; i++) {
		if (g_menu.m_cMenuItem[i] != nullptr) {
			free(g_menu.m_cMenuItem[i]);
			g_menu.m_cMenuItem[i] = nullptr;
		}
	}
	g_menu.m_iCursorPos = 0;
	g_menu.m_uiMenuWidth = 0;
	g_menu.m_uiMenuHeight = 0;
	return;
}

void menu_create(const char *cMenuTitle, int iXPos, int iYPos, void (*vFunc)(int iSelection)) {
	if (g_menu.m_cLabel != nullptr) {
		free(g_menu.m_cLabel);
		g_menu.m_cLabel = nullptr;
	}
	g_menu.m_cLabel = strdup(cMenuTitle);
	g_menu.m_vFunc = vFunc;
	return;
}

void menu_add(const char *cFmt, ...) {
	char cLabel[512];
	va_list vArgs;
	va_start(vArgs, cFmt);
	vsnprintf(cLabel, 512, cFmt, vArgs);
	va_end(vArgs);
	for (int i = 0; i < 16; i++) {
		if (g_menu.m_cMenuItem[i] == nullptr) {
			g_menu.m_cMenuItem[i] = strdup(cLabel);
			if (strlen(cLabel) > g_menu.m_uiMenuWidth) {
				g_menu.m_uiMenuWidth = strlen(cLabel);
			}
			g_menu.m_uiMenuHeight++;
			break;
		}
	}
	return;
}

/* Grabs input in the SDL_PollEvent loop, interprets it.
Menus take input precedence over game input. */
void menu_input(SDL_Event *sdlEvent) {
	input_event ev;
	input_get_event(*sdlEvent, &ev);
	
	switch(ev.m_iKey) {
	case IN_DIRUP:
		if (g_menu.m_iCursorPos > 0) {
			g_menu.m_iCursorPos--;
		}
		break;
	case IN_DIRDOWN:
		if ((unsigned int)g_menu.m_iCursorPos < g_menu.m_uiMenuHeight - 1) {
			g_menu.m_iCursorPos++;
		}
		break;
	case IN_OK:
		if(ev.m_iType == IN_OFF) {
			g_menu.m_vFunc(g_menu.m_iCursorPos);
		}
	}
	return;
}

/* one day we'll replace this with a dialog box. */
void menu_render() {
	if (g_menu.m_cLabel != nullptr) {
		/* Show the menu. */

		/* Top line of menu */
		font_putc(0, 0, 201);
		font_print(16, 0, g_menu.m_cLabel);
		for (unsigned int i = (16 + (strlen(g_menu.m_cLabel) * 16)); i <= ((g_menu.m_uiMenuWidth + 3) * 16); i += 16) {
			font_putc(i, 0, 205);
		}
		font_putc((g_menu.m_uiMenuWidth + 4) * 16, 0, 187);

		font_putc(32, 32 * (g_menu.m_iCursorPos + 1), 16);

		/* Menu Items. */
		for (int i = 0; i < 16; i++) {
			if (g_menu.m_cMenuItem[i] != nullptr) {
				if (i == g_menu.m_iCursorPos) {
					image_setcolormod(g_font.m_image, 0x00, 0xaa, 0x00);
					font_print(48, 32 * (i + 1), g_menu.m_cMenuItem[i]);
					image_setcolormod(g_font.m_image, 0xff, 0xff, 0xff);
				}
				else {
					font_print(48, 32 * (i + 1), g_menu.m_cMenuItem[i]);
				}
			}
		}

		/* Left/Right borders. */
		font_putc(0, (g_menu.m_uiMenuHeight + 1) * 32, 200);

		for (unsigned int i = 0; i <= (g_menu.m_uiMenuHeight) * 2; i++) {
			font_putc(0, 16 * (i + 1), 186);
			font_putc((g_menu.m_uiMenuWidth + 4) * 16, 16 * (i + 1), 186);
		}
		for (unsigned int i = 16; i <= ((g_menu.m_uiMenuWidth + 3) * 16); i += 16) {
			font_putc(i, (g_menu.m_uiMenuHeight + 1) * 32, 205);
		}
		font_putc((g_menu.m_uiMenuWidth + 4) * 16, (g_menu.m_uiMenuHeight + 1) * 32, 188);
	}
	return;
}

void menu_close() {
	if (g_menu.m_cLabel != nullptr) {
		free(g_menu.m_cLabel);
		g_menu.m_cLabel = nullptr;
	}
	for (int i = 0; i < 16; i++) {
		if (g_menu.m_cMenuItem[i] != nullptr) {
			free(g_menu.m_cMenuItem[i]);
			g_menu.m_cMenuItem[i] = nullptr;
		}
	}
	g_menu.m_iCursorPos = 0;
	g_menu.m_uiMenuWidth = 0;
	g_menu.m_uiMenuHeight = 0;
	return;
}

bool menu_is_open() {
	return (g_menu.m_cLabel != nullptr);
}

void menu_destroy() {
	if (g_menu.m_cLabel != nullptr) {
		free(g_menu.m_cLabel);
		g_menu.m_cLabel = nullptr;
	}
	if (g_menu.m_cLuaScript != nullptr) {
		free(g_menu.m_cLuaScript);
		g_menu.m_cLuaScript = nullptr;
	}
	for (int i = 0; i < 16; i++) {
		if (g_menu.m_cMenuItem[i] != nullptr) {
			free(g_menu.m_cMenuItem[i]);
			g_menu.m_cMenuItem[i] = nullptr;
		}
	}
	g_menu.m_iCursorPos = 0;
	g_menu.m_uiMenuWidth = 0;
	g_menu.m_uiMenuHeight = 0;
	return;
}
