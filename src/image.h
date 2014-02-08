#ifndef __IMAGE_H__
#define __IMAGE_H__

int image_load(image *imageDest, char *cFile, bool bColorKey, unsigned int uiRed, unsigned int uiGreen, unsigned int uiBlue);
int image_create_texture(image *imageDest, int iWidth, int iHeight);
int image_setcolormod(image *iImage, unsigned int uiRed, unsigned int uiGreen, unsigned int uiBlue);
int image_draw(image *iImage, rect *rectSource, rect *rectDest);
int image_draw_to(image *imageDest, image *imageSource, rect *rectSource, rect *rectDest);
void image_destroy(image *iImage);

#endif
