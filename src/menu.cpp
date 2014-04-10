#include "include.h"

#define MARGIN 4
#define PADDING_CHARS 2

menu* menu_init(int x, int y) {
	menu *pMenu = (menu*) malloc(sizeof(menu));
	
	pMenu->m_iCursorPos = 0;
	pMenu->m_iX = x;
	pMenu->m_iY = y;
	pMenu->m_iWidth = 0;
	pMenu->m_iHeight = 0;
	pMenu->m_iValueX = 0;
	pMenu->m_aColors = table_new(sizeof(Uint32), 0, 0);
	pMenu->m_aEntries = table_new(sizeof(char*), 0, 0);
	pMenu->m_aValues = table_new(sizeof(char*), 0, 0);
	
	return pMenu;
}

void menu_add_entry(menu *pMenu, const char *fmt, ...) {
	char* cText = (char*) malloc(512);
	if (! cText) {
		debug_print("Out of memory.\n");
		sys_abort();
	}
	va_list vArgs;
	va_start(vArgs, fmt);
	vsnprintf(cText, 512, fmt, vArgs);
	va_end(vArgs);
	
	table_append(pMenu->m_aEntries, &cText);
	cText = nullptr;
	table_append(pMenu->m_aValues, &cText);
	Uint32 textcol = COLOR_MENU_TEXT;
	table_append(pMenu->m_aColors, &textcol);	
}

void menu_set_value(menu *pMenu, const char *fmt, ...) {
	char* cText = (char*) malloc(512);
	if (! cText) {
		debug_print("Out of memory.\n");
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

void menu_clear(menu *pMenu) {
	char *entry;
	for (int i = 0; i < pMenu->m_aEntries->m_len; i++) {
		table_get(pMenu->m_aEntries, i, &entry);
		free(entry);
		table_get(pMenu->m_aValues, i, &entry);
		free(entry);
	}
	table_shrink(pMenu->m_aEntries, pMenu->m_aEntries->m_len);
	table_shrink(pMenu->m_aValues, pMenu->m_aValues->m_len);
}

void menu_auto_resize(menu *pMenu) {
	int maxelem, maxval, len;
	maxelem = maxval = 0;
	
	char **entries = (char**) pMenu->m_aEntries->m_data;
	char **values = (char**) pMenu->m_aValues->m_data;
	for (int i = 0; i < pMenu->m_aEntries->m_len; i++) {
		len = font_text_width(g_font, entries[i]);
		if (len > maxelem) { maxelem = len; }
		
		if (values[i] == nullptr) { continue; }
		len = font_text_width(g_font, values[i]);
		if (len > maxval) { maxval = len; }
	}
	
	if (maxval > 0) {
		pMenu->m_iValueX = MARGIN + maxelem + PADDING_CHARS * font_text_width(g_font, "0");
		pMenu->m_iWidth = pMenu->m_iValueX + maxval + MARGIN;
	} else {
		pMenu->m_iValueX = MARGIN + maxelem + PADDING_CHARS * font_text_width(g_font, "0");
		pMenu->m_iWidth = MARGIN + maxelem + MARGIN;
	}
	
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
	input_get_event(sdlEvent, &ev);
	
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

const char* menu_current_entry(menu* pMenu) {
	const char *str;
	table_get(pMenu->m_aEntries, pMenu->m_iCursorPos, &str);
	return str;
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
			pMenu->m_iX + MARGIN + pMenu->m_iValueX,
			pMenu->m_iY + i * (g_font.m_iGlyphHeight + MARGIN*2) + MARGIN,
			menuValue[i]);
	}
	
	dst.x = pMenu->m_iX;
	dst.y = pMenu->m_iY;
	dst.w = pMenu->m_iWidth;
	dst.h = pMenu->m_iHeight;
	screen_draw_rect(&dst, COLOR_MENU_BORDER);
}

void menu_destroy(menu* pMenu) {
	char* entry;
	for (int i = 0; i < pMenu->m_aEntries->m_len; i++) {
		table_get(pMenu->m_aEntries, i, &entry);
		free(entry);
		table_get(pMenu->m_aValues, i, &entry);
		free(entry);
	}
	table_free(pMenu->m_aEntries);
	table_free(pMenu->m_aValues);
	free(pMenu);
}
