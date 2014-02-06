#include "include.h"

void font_init() {
	debug_print("Loading Font ...\r\n");
	g_font.m_image= image_load("data/images/fonts/437_16x16.bmp", true, 0xff, 0x00, 0xff);
/*

	SDL_Surface *surfTemp = SDL_LoadBMP("data/images/fonts/437_16x16.bmp");
	if (surfTemp == nullptr) {
		printf("font_init(): %s\r\n", SDL_GetError());
		return;
	}
	SDL_SetColorKey(surfTemp, SDL_TRUE, SDL_MapRGB(surfTemp->format, 0xff, 0x00, 0xff));
	g_font.m_sdlFont = SDL_CreateTextureFromSurface(g_sdlRenderer, surfTemp);
	if (g_font.m_sdlFont == nullptr) {
		printf("font_init(): Unable to convert image to texture (why?)\r\n");
		return;
	}
	/*
		This just takes a 256x256 pixel image and calculates the the x/y position
		of each 16x16 character in the font.

		This process is fucked if we're on a machine that doesn't round-down on
		dividing integers.
	*/
	for (int i = 0; i < 256; i++) {
		g_font.m_rectClip[i].x = (i - (16 * (i / 16))) * 16; // I'm sure there's another way to calculate this.
		g_font.m_rectClip[i].y = (i / 16) * 16;
		g_font.m_rectClip[i].w = 16;
		g_font.m_rectClip[i].h = 16;
	}
	return;
}

/* Because va_'s %c is not unsigned! */
void font_putc(int iX, int iY, unsigned char ucChar) {
	g_font.m_rectDraw[0].x = iX;
	g_font.m_rectDraw[0].y = iY;
	g_font.m_rectDraw[0].w = 16;
	g_font.m_rectDraw[0].h = 16;
	image_draw(&g_font.m_image, &g_font.m_rectClip[ucChar], &g_font.m_rectDraw[0]);
	return;
}

void font_print(int iX, int iY, char *cFmt, ...) {
	char cText[512];
	va_list vArgs;
	va_start(vArgs, cFmt);
	vsnprintf(cText, 512, cFmt, vArgs);
	va_end(vArgs);
	if (g_font.m_cText != nullptr) {
		free(g_font.m_cText);
		g_font.m_cText = nullptr;
	}
	g_font.m_cText = (char *)malloc(sizeof(char) * strlen(cText));
	memcpy(g_font.m_cText, cText, sizeof(unsigned char) * strlen(cText));

	/* Calculates the x position of each rectangle to draw. */
	for (unsigned int i = 0; i < strlen(cText); i++) {
		g_font.m_rectDraw[i].x = iX + (i * 16);
		g_font.m_rectDraw[i].y = iY;
		g_font.m_rectDraw[i].w = 16;
		g_font.m_rectDraw[i].h = 16;
		image_draw(&g_font.m_image, &g_font.m_rectClip[cText[i]], &g_font.m_rectDraw[i]);
	}
	return;
}

void font_destroy() {
	if (g_font.m_image.m_image != nullptr) {
		image_destroy(&g_font.m_image);
	}
	return;
}
