#ifndef __IMAGE_H__
#define __IMAGE_H__

image* image_load(const char *cFile, bool bColorKey, unsigned int uiRed, unsigned int uiGreen, unsigned int uiBlue);
void image_get_size(image* img, int *iWidth, int *iHeight);
image* image_create_texture(int iWidth, int iHeight);
int image_setcolormod(image *iImage, Uint32 color);
int image_draw(image *iImage, rect *rectSource, rect *rectDest);
void image_draw_to(image *imageDest, image *imageSource, rect *rectSource, rect *rectDest);
void image_draw_rec(rect *rectDest, int iRed, int iGreen, int iBlue);
void image_draw_fill_rect(rect *rectDest, int iRed, int iGreen, int iBlue);
void image_draw_rect_to(image *imageDest, rect *rectDest, int iRed, int iGreen, int iBlue);
void image_draw_fill_rect_to(image *imageDest, rect *rectDest, int iRed, int iGreen, int iBlue);
void image_destroy(image *iImage);

#endif
