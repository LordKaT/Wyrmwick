#ifndef __FONT_H__
#define __FONT_H__

font font_init(const char *path);
void font_putc(font Font, int iX, int iY, unsigned char ucChar);
void font_print(font Font, int iX, int iY, const char *cFmt, ...);
void font_putc_to(font Font, image *imageDest, int iX, int iY, unsigned char ucChar);
void font_print_to(font Font, image *imageDest, int iX, int iY, const char *cFmt, ...);
void font_destroy(font Font);

#endif
