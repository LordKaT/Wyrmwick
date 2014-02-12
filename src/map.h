#ifndef __MAP_H__
#define __MAP_H__

void map_init();
void map_draw();
void map_draw_view();
void map_draw_grid();
void map_draw_grid_view(); 
void map_load(const char *cMap);
void map_save();
void map_move(int iX, int iY);
void map_render();
void map_destroy();

#endif
