#ifndef __AUDIO_H__
#define __AUDIO_H__

void audio_init();
int audio_load_music(audio *audioChunk, const char *cFile);
int audio_play_music(audio *audioChunk);
void audio_destroy();

#endif
