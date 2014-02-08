#include "include.h"

int image_load(image *imageDest, char *cFile, bool bColorKey, unsigned int uiRed, unsigned int uiGreen, unsigned int uiBlue) {
#ifdef SCREEN_SDL2
	SDL_Surface *surfTemp = SDL_LoadBMP(cFile);
	if (surfTemp == nullptr) {
		printf("image_load(): %s\r\n", SDL_GetError());
		return 0;
	}

	if (bColorKey == true) {
		SDL_SetColorKey(surfTemp, SDL_TRUE, SDL_MapRGB(surfTemp->format, uiRed, uiGreen, uiBlue));
	}

	SDL_Texture *texTemp = SDL_CreateTextureFromSurface(g_sdlRenderer, surfTemp);
	if (texTemp == nullptr) {
		printf("image_load(): %s\r\n", SDL_GetError());
		return 0;
	}
	imageDest->m_image = texTemp;
	SDL_FreeSurface(surfTemp);
	return 0;
#endif
}

int image_create_texture(image *imageDest, int iWidth, int iHeight) {
#ifdef SCREEN_SDL2
	if (imageDest->m_image == nullptr) {
		imageDest->m_image = SDL_CreateTexture(g_sdlRenderer, SDL_GetWindowPixelFormat(g_sdlWindow), SDL_TEXTUREACCESS_TARGET, iWidth, iHeight);
		if (imageDest->m_image == nullptr) {
			debug_print("image_draw_to(): %s\r\n", SDL_GetError());
			return 0;
		}
	}
#endif
	return 0;
}

int image_setcolormod(image *iImage, unsigned int uiRed, unsigned int uiGreen, unsigned int uiBlue) {
#ifdef SCREEN_SDL2
	return SDL_SetTextureColorMod(iImage->m_image, uiRed, uiGreen, uiBlue);
#endif
}

int image_draw(image *iImage, rect *rectSource, rect *rectDest) {
#ifdef SCREEN_SDL2
	return SDL_RenderCopy(g_sdlRenderer, iImage->m_image, rectSource, rectDest);
#endif
}

int image_draw_to(image *imageDest, image *imageSource, rect *rectSource, rect *rectDest) {
#ifdef SCREEN_SDL2
	int iRet = 0;
	if (imageDest->m_image == nullptr) {
		debug_print("image_draw_to(): image is null. How do I draw to it?\r\n");
		return 0;
	}
	iRet = SDL_SetTextureBlendMode(imageDest->m_image, SDL_BLENDMODE_BLEND);
	if (iRet < 0) {
		debug_print("image_draw_to(): %s\r\n", SDL_GetError());
		return iRet;
	}
	iRet = SDL_SetRenderTarget(g_sdlRenderer, imageDest->m_image);
	if (iRet < 0) {
		debug_print("image_draw_to(): %s\r\n", SDL_GetError());
		return iRet;
	}
	iRet = SDL_RenderCopy(g_sdlRenderer, imageSource->m_image, rectSource, rectDest);
	if (iRet < 0) {
		debug_print("image_draw_to(): %s\r\n", SDL_GetError());
		return iRet;
	}
	iRet = SDL_SetRenderTarget(g_sdlRenderer, nullptr);
	if (iRet < 0) {
		debug_print("image_draw_to(): %s\r\n", SDL_GetError());
		return iRet;
	}
	return iRet;
#endif
}

int image_draw_rect_to(image *imageDest, rect *rectDest) {
#ifdef SCREEN_SDL2
	int iRet = 0;
	if (imageDest->m_image == nullptr) {
		debug_print("image_draw_rect_to(): image is null. How do I draw to it?\r\n");
		return iRet;
	}
	iRet = SDL_SetTextureBlendMode(imageDest->m_image, SDL_BLENDMODE_BLEND);
	if (iRet < 0) {
		debug_print("image_draw_rect_to(): %s\r\n", SDL_GetError());
		return iRet;
	}
	iRet = SDL_SetRenderTarget(g_sdlRenderer, imageDest->m_image);
	if (iRet < 0) {
		debug_print("image_draw_rect_to(): %s\r\n", SDL_GetError());
		return iRet;
	}
	iRet = SDL_RenderDrawRect(g_sdlRenderer, rectDest);
	if (iRet < 0) {
		debug_print("image_draw_rect_to(): %s\r\n", SDL_GetError());
		return iRet;
	}
	iRet = SDL_SetRenderTarget(g_sdlRenderer, nullptr);
	if (iRet < 0) {
		debug_print("image_draw_rect_to(): %s\r\n", SDL_GetError());
		return iRet;
	}
	return iRet;
#endif
}

void image_destroy(image *iImage) {
#ifdef SCREEN_SDL2
	SDL_DestroyTexture(iImage->m_image);
#endif
	return;
}
