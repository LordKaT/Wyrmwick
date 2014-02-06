#ifndef __IMAGE_H__
#define __IMAGE_H__

image image_load(char *cFile, bool bColorKey, unsigned int uiRed, unsigned int uiGreen, unsigned int uiBlue);
int image_setcolormod(image *iImage, unsigned int uiRed, unsigned int uiGreen, unsigned int uiBlue);
int image_draw(image *iImage, rect *rectSource, rect *rectDest);
int image_draw_to(image *imageDest, image *imageSource, rect *rectSource, rect *rectDest);
void image_destroy(image *iImage);

#endif
