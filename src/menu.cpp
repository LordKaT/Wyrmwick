#include "include.h"

menu* menu_init(const char *cMenuTitle, int xPos, int yPos) {
	menu *pMenu = (menu*) malloc(sizeof(menu));
	
	pMenu->m_cLabel = strdup(cMenuTitle);
	pMenu->m_iCursorPos = 0;
	pMenu->m_iMaxWidth = 0;
	pMenu->m_aEntries = table_new(sizeof(char*), 0, 0);
	
	pMenu->m_x = xPos;
	pMenu->m_y = yPos;
	
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
	int len = strlen(cText);
	if (len > pMenu->m_iMaxWidth) { pMenu->m_iMaxWidth = len; }
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

/* one day we'll replace this with a dialog box. */
// FIXME: Hardcoded 16x16 font
// FIXME: Actually draw the menu where it's supposed to be.
void menu_render(menu* pMenu) {
	int numEntries = pMenu->m_aEntries->m_len;
	/* Show the menu. */

	/* Top line of menu */
	font_putc(0, 0, 201);
	font_print(16, 0, pMenu->m_cLabel);
	for (int i = (16 + (strlen(pMenu->m_cLabel) * 16)); i <= ((pMenu->m_iMaxWidth + 3) * 16); i += 16) {
		font_putc(i, 0, 205);
	}
	font_putc((pMenu->m_iMaxWidth + 4) * 16, 0, 187);

	font_putc(32, 32 * (pMenu->m_iCursorPos + 1), 16);

	char **menuItem = (char**) pMenu->m_aEntries->m_data;
	for (int i = 0; i < pMenu->m_aEntries->m_len; i++) {
		if (i == pMenu->m_iCursorPos) {
			image_setcolormod(g_font.m_image, 0x00, 0xaa, 0x00);
			font_print(48, 32 * (i + 1), menuItem[i]);
			image_setcolormod(g_font.m_image, 0xff, 0xff, 0xff);
		}
		else {
			font_print(48, 32 * (i + 1), menuItem[i]);
		}
	}

	/* Left/Right borders. */
	font_putc(0, (numEntries + 1) * 32, 200);

	for (int i = 0; i <= numEntries * 2; i++) {
		font_putc(0, 16 * (i + 1), 186);
		font_putc((pMenu->m_iMaxWidth + 4) * 16, 16 * (i + 1), 186);
	}
	for (int i = 16; i <= ((pMenu->m_iMaxWidth + 3) * 16); i += 16) {
		font_putc(i, (numEntries + 1) * 32, 205);
	}
	font_putc((pMenu->m_iMaxWidth + 4) * 16, (numEntries + 1) * 32, 188);
	return;
}

void menu_destroy(menu* pMenu) {
	char* entry;
	free(pMenu->m_cLabel);
	for(int i = 0; i < pMenu->m_aEntries->m_len; i++) {
		table_get(pMenu->m_aEntries, i, &entry);
		free(entry);
	}
	table_free(pMenu->m_aEntries);
	free(pMenu);

	return;
}
