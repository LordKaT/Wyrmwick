#ifndef __PLAYER_H__
#define __PLAYER_H__

void player_init();
void player_animate(void *vData);
void player_input(SDL_Event *sdlEvent);
bool player_collide();
void player_render();
void player_destroy();

#endif
