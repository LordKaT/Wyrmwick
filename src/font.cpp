#include "include.h"

rect _glyph_rect(font Font, int index) {
	rect r;
	r.x = (index % 16) * Font.m_iGlyphWidth;
	r.y = (index / 16) * Font.m_iGlyphHeight;
	r.w = Font.m_iGlyphWidth;
	r.h = Font.m_iGlyphHeight;
	return r;
}

void font_init() {
	debug_print("Loading Font ...\r\n");
	g_font.m_image = image_load("data/images/fonts/437_16x16.bmp", true, 0xff, 0x00, 0xff);

	int iW, iH;
	image_get_size(g_font.m_image, &iW, &iH);
	if (iW % 16 != 0 || iH % 16 != 0) {
		debug_print("Font has invalid dimensions.\r\n");
	}
	g_font.m_iGlyphWidth = iW / 16;
	g_font.m_iGlyphHeight = iH / 16;
	return;
}

/* Because va_'s %c is not unsigned! */
void font_putc(int iX, int iY, unsigned char ucChar) {
	rect srcRect = _glyph_rect(g_font, ucChar);
	rect dstRect = {iX, iY, g_font.m_iGlyphWidth, g_font.m_iGlyphHeight};
	image_draw(g_font.m_image, &srcRect, &dstRect);
	return;
}

void font_print(int iX, int iY, const char *cFmt, ...) {
	char cText[512];
	va_list vArgs;
	va_start(vArgs, cFmt);
	vsnprintf(cText, 512, cFmt, vArgs);
	va_end(vArgs);
	
	rect srcRect;
	rect dstRect = {iX, iY, g_font.m_iGlyphWidth, g_font.m_iGlyphHeight};
	
	for (char *ch = cText; *ch != '\0'; ch++) {
		srcRect = _glyph_rect(g_font, *ch);
		image_draw(g_font.m_image, &srcRect, &dstRect);
		dstRect.x += g_font.m_iGlyphWidth;
	}
	return;
}

/* font_putc_to and font_print_to used for printing to another texture. */
void font_putc_to(image *imageDest, int iX, int iY, unsigned char ucChar) {
	rect srcRect = _glyph_rect(g_font, ucChar);
	rect dstRect = {iX, iY, g_font.m_iGlyphWidth, g_font.m_iGlyphHeight};
	image_draw_to(imageDest, g_font.m_image, &srcRect, &dstRect);
	return;
}

void font_print_to(image *imageDest, int iX, int iY, const char *cFmt, ...) {
	char cText[512];
	va_list vArgs;
	va_start(vArgs, cFmt);
	vsnprintf(cText, 512, cFmt, vArgs);
	va_end(vArgs);

	rect srcRect;
	rect dstRect = {iX, iY, g_font.m_iGlyphWidth, g_font.m_iGlyphHeight};
	
	for (char *ch = cText; *ch != '\0'; ch++) {
		srcRect = _glyph_rect(g_font, *ch);
		image_draw_to(imageDest, g_font.m_image, &srcRect, &dstRect);
		dstRect.x += g_font.m_iGlyphWidth;
	}
	return;
}

void font_destroy() {
	if (g_font.m_image != nullptr) {
		image_destroy(g_font.m_image);
	}
	return;
}
