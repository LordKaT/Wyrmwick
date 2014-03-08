#ifndef __FONT_H__
#define __FONT_H__

font font_init(const char *path);

// font_putc, font_print, font_putc_to and font_print_to return the width of the rendered text, in pixels.

int font_putc(font Font, int iX, int iY, unsigned char ucChar);
int font_print(font Font, int iX, int iY, const char *cFmt, ...);
int font_putc_to(font Font, image *imageDest, int iX, int iY, unsigned char ucChar);
int font_print_to(font Font, image *imageDest, int iX, int iY, const char *cFmt, ...);

int font_text_width(font Font, const char *sText);

void font_destroy(font Font);

#endif
