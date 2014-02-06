#include "include.h"

#define AUDIO_SDLMIXER

void audio_init() {
#ifdef AUDIO_SDLMIXER
	Mix_Init(MIX_INIT_MP3);
	printf("Mix_Init: %s\r\n", Mix_GetError());
#endif
	return;
}

void audio_destroy() {
#ifdef AUDIO_SDLMIXER
	while (Mix_Init(0)) {
		Mix_Quit();
	}
#endif
	return;
}
