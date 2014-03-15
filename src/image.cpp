#include "include.h"

image* image_load(const char *cFile, bool bColorKey, unsigned int uiRed, unsigned int uiGreen, unsigned int uiBlue) {
#ifdef SCREEN_SDL2
	SDL_Surface *surfTemp = IMG_Load(cFile);
	if (surfTemp == nullptr) {
		printf("image_load(): %s\n", SDL_GetError());
		return nullptr;
	}

	if (bColorKey == true) {
		SDL_SetColorKey(surfTemp, SDL_TRUE, SDL_MapRGB(surfTemp->format, uiRed, uiGreen, uiBlue));
	}

	SDL_Texture *texTemp = SDL_CreateTextureFromSurface(g_sdlRenderer, surfTemp);
	if (texTemp == nullptr) {
		printf("image_load(): %s\n", SDL_GetError());
		return nullptr;
	}
	
	SDL_FreeSurface(surfTemp);
	return texTemp;
#endif
}

void image_get_size(image *img, int *iWidth, int *iHeight) {
#ifdef SCREEN_SDL2
	Uint32 junk1;
	int junk2;
	int err = SDL_QueryTexture(img, &junk1, &junk2, iWidth, iHeight);
	if (err) {
		debug_print("Failed to get texture size: %s\n", SDL_GetError());
		// HALT HERE
	}
#endif
}

image* image_create_texture(int iWidth, int iHeight) {
#ifdef SCREEN_SDL2
	image *imageDest;
	imageDest = SDL_CreateTexture(g_sdlRenderer, SDL_GetWindowPixelFormat(g_sdlWindow), SDL_TEXTUREACCESS_TARGET, iWidth, iHeight);
	if (imageDest == nullptr) {
		debug_print("image_draw_to(): %s\n", SDL_GetError());
		return nullptr;
	}
	return imageDest;
#endif
	return nullptr;
}

int image_setcolormod(image *iImage, Uint32 color) {
#ifdef SCREEN_SDL2
	return SDL_SetTextureColorMod(iImage, (color&0xff000000)>>24, (color&0xff0000)>>16, (color&0xff00)>>8);
#endif
}

int image_draw(image *iImage, rect *rectSource, rect *rectDest) {
#ifdef SCREEN_SDL2
	return SDL_RenderCopy(g_sdlRenderer, iImage, rectSource, rectDest);
#endif
}

void image_draw_to(image *imageDest, image *imageSource, rect *rectSource, rect *rectDest) {
#ifdef SCREEN_SDL2
	sdl_must(SDL_SetTextureBlendMode(imageDest, SDL_BLENDMODE_BLEND));
	sdl_must(SDL_SetRenderTarget(g_sdlRenderer, imageDest));
	sdl_must(SDL_RenderCopy(g_sdlRenderer, imageSource, rectSource, rectDest));
	sdl_must(SDL_SetRenderTarget(g_sdlRenderer, nullptr));
#endif
}

void image_draw_rect(rect *rectDest, int iRed, int iGreen, int iBlue) {
#ifdef SCREEN_SDL2
	sdl_must(SDL_SetRenderDrawColor(g_sdlRenderer, iRed, iGreen, iBlue, 255));
	sdl_must(SDL_RenderDrawRect(g_sdlRenderer, rectDest));
	sdl_must(SDL_SetRenderDrawColor(g_sdlRenderer, 0, 0, 0, 255));
#endif
}

void image_draw_fill_rect(rect *rectDest, int iRed, int iGreen, int iBlue) {
#ifdef SCREEN_SDL2
	sdl_must(SDL_SetRenderDrawColor(g_sdlRenderer, iRed, iGreen, iBlue, 255));
	sdl_must(SDL_RenderFillRect(g_sdlRenderer, rectDest));
	sdl_must(SDL_SetRenderDrawColor(g_sdlRenderer, 0, 0, 0, 255));
#endif
}

void image_draw_rect_to(image *imageDest, rect *rectDest, int iRed, int iGreen, int iBlue) {
#ifdef SCREEN_SDL2
	sdl_must(SDL_SetTextureBlendMode(imageDest, SDL_BLENDMODE_BLEND));
	sdl_must(SDL_SetRenderTarget(g_sdlRenderer, imageDest));
	sdl_must(SDL_SetRenderDrawColor(g_sdlRenderer, iRed, iGreen, iBlue, 255));
	sdl_must(SDL_RenderDrawRect(g_sdlRenderer, rectDest));
	sdl_must(SDL_SetRenderDrawColor(g_sdlRenderer, 0, 0, 0, 255));
	sdl_must(SDL_SetRenderTarget(g_sdlRenderer, nullptr));
#endif
}

void image_draw_fill_rect_to(image *imageDest, rect *rectDest, int iRed, int iGreen, int iBlue) {
#ifdef SCREEN_SDL2
	if (imageDest == nullptr) {
		debug_print("image_draw_rect_to(): image is null. How do I draw to it?\n");
		sys_abort();
	}
	sdl_must(SDL_SetTextureBlendMode(imageDest, SDL_BLENDMODE_BLEND));
	sdl_must(SDL_SetRenderTarget(g_sdlRenderer, imageDest));
	sdl_must(SDL_SetRenderDrawColor(g_sdlRenderer, iRed, iGreen, iBlue, 255));
	sdl_must(SDL_RenderFillRect(g_sdlRenderer, rectDest));
	sdl_must(SDL_SetRenderDrawColor(g_sdlRenderer, 0, 0, 0, 255));
	sdl_must(SDL_SetRenderTarget(g_sdlRenderer, nullptr));
#endif
}

void image_destroy(image *iImage) {
#ifdef SCREEN_SDL2
	SDL_DestroyTexture(iImage);
#endif
	return;
}
