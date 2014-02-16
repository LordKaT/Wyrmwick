#include "include.h"

void debug_print(const char *cFmt, ...) {
#ifdef DEBUG
	char cText[512];
	va_list vArgs;
	va_start(vArgs, cFmt);
	vsnprintf(cText, 512, cFmt, vArgs);
	va_end(vArgs);
	printf("%s", cText);
#endif
	return;
}

/* Entered from menu_input (menu.cpp). */
void debug_menu_func(int iSelection) {
	debug_print("debug_menu_func: %i\r\n", iSelection);
	switch (iSelection) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			map_editor_init();
			break;
		case 3:
			break;
		default:
			break;
	}
	return;
}
