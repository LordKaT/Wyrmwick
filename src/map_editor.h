#ifndef __MAP_EDITOR_H__
#define __MAP_EDITOR_H__

void map_editor_push(state_stack* stack);
void map_editor_init(state_stack* stack);
void map_editor_input(state_stack* stack, SDL_Event *sdlEvent);
void map_editor_draw_walk();
void map_editor_draw_walk_view();
void map_editor_render(state_stack* stack);
void map_editor_destroy(state_stack* stack);
#endif
