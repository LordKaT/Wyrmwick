#ifndef __FONT_H__
#define __FONT_H__

void font_init();
void font_putc(int iX, int iY, unsigned char ucChar);
void font_print(int iX, int iY, char *cFmt, ...);
void font_putc_to(image *imageDest, int iX, int iY, unsigned char ucChar);
void font_print_to(image *imageDest, int iX, int iY, char *cFmt, ...);
void font_destroy();

#endif
