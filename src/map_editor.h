#ifndef __MAP_EDITOR_H__
#define __MAP_EDITOR_H__

void map_editor_init();
void map_editor_destroy();
void map_editor_input(SDL_Event *sdlEvent);
void map_editor_draw_walk();
void map_editor_draw_walk_view();
void map_editor_render();

#endif
