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

