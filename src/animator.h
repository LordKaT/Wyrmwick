#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__

void animator_init();
void animator_destroy();
void animator_animate(void *vData);
Uint32 animator_callback(Uint32 iInterval, void *vParam);
void animator_input(SDL_Event *sdlEvent);
void animator_add(animation *aAnim, int iInterval, void *vParam);

#endif
