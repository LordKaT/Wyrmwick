#ifndef __SCREEN_H__
#define __SCREEN_H__

void screen_config_settings(settings* st, void* udata);
void screen_init();
void screen_fill_rect(rect *dst, Uint32 color);
void screen_draw_rect(rect *dst, Uint32 color);
void screen_draw_line(int x1, int y1, int x2, int y2, Uint32 color);
void screen_clear();
void screen_present();
void screen_destroy();

#endif
