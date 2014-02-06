#include "include.h"

void sdl2mixer_init() {
	if (Mix_Init(MIX_INIT_MP3) < 0) {
		printf("sdl2mixer_init(): %s\r\n", Mix_GetError());
		return;
	}
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4069);
	return;
}

void sdl2mixer_destroy() {
	while (Mix_Init(0)) {
		Mix_Quit();
	}
	return;
}
