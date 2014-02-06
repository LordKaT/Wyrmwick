#include "include.h"

void audio_init() {
#ifdef AUDIO_SDL2MIXER
	sdl2mixer_init();
#endif
	return;
}

int audio_load_music(audio *audioChunk, char *cFile) {
#ifdef AUDIO_SDL2MIXER
	audioChunk->m_music = Mix_LoadMUS(cFile);
	if (audioChunk->m_music == nullptr) {
		debug_print("audio_load_music(): %s\r\n", Mix_GetError());
		return 0;
	}
#endif
	return 0;
}

int audio_play_music(audio *audioChunk) {
#ifdef AUDIO_SDL2MIXER
	Mix_PlayMusic(audioChunk->m_music, 0);
#endif
	return 0;
}

void audio_destroy() {
#ifdef AUDIO_SDL2MIXER
	sdl2mixer_destroy();
#endif
	return;
}
