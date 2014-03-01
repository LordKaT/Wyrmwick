#include "include.h"

#define MARGIN 4

menu* menu_init() {
	menu *pMenu = (menu*) malloc(sizeof(menu));
	
	pMenu->m_iCursorPos = 0;
	pMenu->m_iWidth = 0;
	pMenu->m_iHeight = 0;
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
	pMenu->m_iHeight = pMenu->m_aEntries->m_len *  (g_font.m_iGlyphHeight + MARGIN*2);
}

int menu_input(menu* pMenu, SDL_Event *sdlEvent) {
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
	}
	return -1;
}

void menu_render(menu* pMenu, int x, int y) {
	rect dst;
	Uint32 color;
	int numEntries = pMenu->m_aEntries->m_len;
	
	dst = {x, y, pMenu->m_iWidth+2, pMenu->m_iHeight+2};
	screen_draw_rect(&dst, 0xb8fffbff);
	dst = {x+1, y+1, pMenu->m_iWidth, pMenu->m_iHeight};
	screen_fill_rect(&dst, 0x003380ff);
	
	char **menuItem = (char**) pMenu->m_aEntries->m_data;
	for (int i = 0; i < numEntries; i++) {
		if (i == pMenu->m_iCursorPos) {
			dst = {x+1, y + i * (g_font.m_iGlyphHeight + MARGIN*2) + 1,
				pMenu->m_iWidth, g_font.m_iGlyphHeight+MARGIN*2};
			screen_fill_rect(&dst, 0x5f8dd3ff);
		}
		
		table_get(pMenu->m_aColors, i, &color);
		image_setcolormod(g_font.m_image, color);
		font_print(g_font, x + 1 + MARGIN, y + 1 + i * (g_font.m_iGlyphHeight + MARGIN*2) + MARGIN, menuItem[i]);
	}
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
