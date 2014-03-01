#ifndef __MENU_H__
#define __MENU_H__

menu* menu_init();
void menu_add_entry(menu *pMenu, const char *fmt, ...);
void menu_set_value(menu *pMenu, const char *fmt, ...);
// menu_input returns the index of the chosen entry, or -1 if no entry was chosen.
void menu_auto_resize(menu *pMenu);
int menu_input(menu* pMenu, SDL_Event *sdlEvent);
void menu_render(menu* pMenu, int x, int y);
void menu_destroy(menu* pMenu);


#endif
