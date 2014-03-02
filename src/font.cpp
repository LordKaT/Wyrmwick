#include "include.h"

rect _glyph_rect(font Font, int index) {
	rect r;
	r.x = (index % 16) * Font.m_iGlyphWidth;
	r.y = (index / 16) * Font.m_iGlyphHeight;
	r.w = Font.m_iGlyphWidth;
	r.h = Font.m_iGlyphHeight;
	return r;
}

font font_init(const char *path) {
	font Font;
	debug_print("Loading Font ...\r\n");
	Font.m_image = image_load(path, true, 0xff, 0x00, 0xff);

	int iW, iH;
	image_get_size(Font.m_image, &iW, &iH);
	if (iW % 16 != 0 || iH % 16 != 0) {
		debug_print("Font has invalid dimensions.\r\n");
	}
	Font.m_iGlyphWidth = iW / 16;
	Font.m_iGlyphHeight = iH / 16;
	return Font;
}

/* Because va_'s %c is not unsigned! */
void font_putc(font Font, int iX, int iY, unsigned char ucChar) {
	rect srcRect = _glyph_rect(Font, ucChar);
	rect dstRect = {iX, iY, Font.m_iGlyphWidth, Font.m_iGlyphHeight};
	image_draw(Font.m_image, &srcRect, &dstRect);
	return;
}

void font_print(font Font, int iX, int iY, const char *cFmt, ...) {
	char cText[512];
	va_list vArgs;
	va_start(vArgs, cFmt);
	vsnprintf(cText, 512, cFmt, vArgs);
	va_end(vArgs);
	
	rect srcRect;
	rect dstRect = {iX, iY, Font.m_iGlyphWidth, Font.m_iGlyphHeight};
	
	for (char *ch = cText; *ch != '\0'; ch++) {
		srcRect = _glyph_rect(Font, *ch);
		image_draw(Font.m_image, &srcRect, &dstRect);
		dstRect.x += Font.m_iGlyphWidth;
	}
	return;
}

/* font_putc_to and font_print_to used for printing to another texture. */
void font_putc_to(font Font, image *imageDest, int iX, int iY, unsigned char ucChar) {
	rect srcRect = _glyph_rect(Font, ucChar);
	rect dstRect = {iX, iY, Font.m_iGlyphWidth, Font.m_iGlyphHeight};
	image_draw_to(imageDest, Font.m_image, &srcRect, &dstRect);
	return;
}

void font_print_to(font Font, image *imageDest, int iX, int iY, const char *cFmt, ...) {
	char cText[512];
	va_list vArgs;
	va_start(vArgs, cFmt);
	vsnprintf(cText, 512, cFmt, vArgs);
	va_end(vArgs);

	rect srcRect;
	rect dstRect = {iX, iY, Font.m_iGlyphWidth, Font.m_iGlyphHeight};
	
	for (char *ch = cText; *ch != '\0'; ch++) {
		srcRect = _glyph_rect(Font, *ch);
		image_draw_to(imageDest, Font.m_image, &srcRect, &dstRect);
		dstRect.x += Font.m_iGlyphWidth;
	}
	return;
}

void font_destroy(font Font) {
	if (Font.m_image != nullptr) {
		image_destroy(Font.m_image);
	}
	return;
}
