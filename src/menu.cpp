#include "include.h"

#define MARGIN 4

menu* menu_init(int x, int y) {
	menu *pMenu = (menu*) malloc(sizeof(menu));
	
	pMenu->m_iCursorPos = 0;
	pMenu->m_iX = x;
	pMenu->m_iY = y;
	pMenu->m_iWidth = 0;
	pMenu->m_iHeight = 0;
	pMenu->m_iMaxEntryLen = 0;
	pMenu->m_aColors = table_new(sizeof(color_rgba), 0, 0);
	pMenu->m_aEntries = table_new(sizeof(char*), 0, 0);
	pMenu->m_aValues = table_new(sizeof(char*), 0, 0);
	
	return pMenu;
}

void menu_add_entry(menu *pMenu, const char *fmt, ...) {
	char* cText = (char*) malloc(512);
	if (! cText) {
		debug_print("Out of memory.\r\n");
		sys_abort();
	}
	va_list vArgs;
	va_start(vArgs, fmt);
	vsnprintf(cText, 512, fmt, vArgs);
	va_end(vArgs);
	
	if ((int)strlen(cText) > pMenu->m_iMaxEntryLen) { pMenu->m_iMaxEntryLen = strlen(cText); }
	
	table_append(pMenu->m_aEntries, &cText);
	cText = nullptr;
	table_append(pMenu->m_aValues, &cText);
	color_rgba col = {255, 255, 255, 255};
	table_append(pMenu->m_aColors, &col);	
}

void menu_set_value(menu *pMenu, const char *fmt, ...) {
	char* cText = (char*) malloc(512);
	if (! cText) {
		debug_print("Out of memory.\r\n");
		sys_abort();
	}
	va_list vArgs;
	va_start(vArgs, fmt);
	vsnprintf(cText, 512, fmt, vArgs);
	va_end(vArgs);
	
	table_put(pMenu->m_aValues, pMenu->m_aValues->m_len-1, &cText);
}

void menu_set_color(menu *pMenu, Uint32 color) {
	table_put(pMenu->m_aColors, pMenu->m_aColors->m_len-1, &color);
}

void menu_auto_resize(menu *pMenu) {
	int maxlene, maxlenv, len;
	maxlene = maxlenv = 0;
	
	char **entries = (char**) pMenu->m_aEntries->m_data;
	char **values = (char**) pMenu->m_aValues->m_data;
	for (int i = 0; i < pMenu->m_aEntries->m_len; i++) {
		len = strlen(entries[i]);
		if (len > maxlene) { maxlene = len; }
		
		if (values[i] == nullptr) { continue; }
		len = strlen(values[i]);
		if (len > maxlenv) { maxlenv = len; }
	}
	
	pMenu->m_iWidth = (maxlene + maxlenv) * g_font.m_iGlyphWidth + MARGIN*2;
	if (maxlenv != 0) { pMenu->m_iWidth += 1; }
	pMenu->m_iHeight = pMenu->m_aEntries->m_len *  (g_font.m_iGlyphHeight + MARGIN*2);
}

int menu_input(menu* pMenu, SDL_Event *sdlEvent) {
	if (sdlEvent->type == SDL_MOUSEMOTION) {
		int x, y;
		x = sdlEvent->motion.x - pMenu->m_iX;
		y = sdlEvent->motion.y - pMenu->m_iY;
		if (x < 0 || y < 0) { return -1; }
		if (x > pMenu->m_iWidth || y > pMenu->m_iHeight) { return -1; }
		pMenu->m_iCursorPos = y / (g_font.m_iGlyphHeight+MARGIN*2);
		return -1;
	}
	
	if (sdlEvent->type == SDL_MOUSEBUTTONUP) {
		if (sdlEvent->button.button != SDL_BUTTON_LEFT) { return -1; }
		int x, y;
		x = sdlEvent->button.x - pMenu->m_iX;
		y = sdlEvent->button.y - pMenu->m_iY;
		if (x < 0 || y < 0) { return -1; }
		if (x > pMenu->m_iWidth || y > pMenu->m_iHeight) { return -1; }
		return pMenu->m_iCursorPos;
	}
	
	input_event ev;
	input_get_event(*sdlEvent, &ev);
	
	switch (ev.m_iKey) {
	case IN_DIRUP:
		if (pMenu->m_iCursorPos > 0) {
			pMenu->m_iCursorPos--;
		}
		break;
	case IN_DIRDOWN:
		if (pMenu->m_iCursorPos < pMenu->m_aEntries->m_len - 1) {
			pMenu->m_iCursorPos++;
		}
		break;
	case IN_OK:
		if (ev.m_iType == IN_OFF) {
			return pMenu->m_iCursorPos;
		}
		break;
	default:
		break;
	}
	return -1;
}

void menu_render(menu* pMenu) {
	Uint32 color;
	int numEntries = pMenu->m_aEntries->m_len;
	rect dst = {pMenu->m_iX, pMenu->m_iY, pMenu->m_iWidth, pMenu->m_iHeight};

	screen_fill_rect(&dst, 0x003380ff);
	
	char **menuItem = (char**) pMenu->m_aEntries->m_data;
	char **menuValue = (char**) pMenu->m_aValues->m_data;
	for (int i = 0; i < numEntries; i++) {
		if (i == pMenu->m_iCursorPos) {
			// quick way wouldn't compile.
			dst.x = pMenu->m_iX;
			dst.y = pMenu->m_iY + i * (g_font.m_iGlyphHeight + MARGIN*2);
			dst.w = pMenu->m_iWidth;
			dst.h = g_font.m_iGlyphHeight+MARGIN * 2;
			screen_fill_rect(&dst, 0x5f8dd3ff);
		}
		
		table_get(pMenu->m_aColors, i, &color);
		image_setcolormod(g_font.m_image, color);
		font_print(g_font, pMenu->m_iX + MARGIN, pMenu->m_iY + i * (g_font.m_iGlyphHeight + MARGIN*2) + MARGIN, menuItem[i]);
		if (menuValue[i] == nullptr) { continue; }
		font_print(g_font,
			pMenu->m_iX + MARGIN + pMenu->m_iMaxEntryLen * g_font.m_iGlyphWidth,
			pMenu->m_iY + i * (g_font.m_iGlyphHeight + MARGIN*2) + MARGIN,
			menuValue[i]);
	}
	// quick way wouldn't compile.
	dst.x = pMenu->m_iX;
	dst.y = pMenu->m_iY;
	dst.w = pMenu->m_iWidth;
	dst.h = pMenu->m_iHeight;
	screen_draw_rect(&dst, 0xb8fffbff);
}

void menu_destroy(menu* pMenu) {
	char* entry;
	for (int i = 0; i < pMenu->m_aEntries->m_len; i++) {
		table_get(pMenu->m_aEntries, i, &entry);
		free(entry);
	}
	table_free(pMenu->m_aEntries);
	free(pMenu);

	return;
}
