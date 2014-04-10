#ifndef __MENU_H__
#define __MENU_H__

menu* menu_init(int x, int y);
void menu_add_entry(menu *pMenu, const char *fmt, ...);
void menu_set_value(menu *pMenu, const char *fmt, ...);
void menu_set_color(menu *pMenu, Uint32 color);
void menu_clear(menu *pMenu);
// menu_auto_resize resizes the menu to fit the entries.
void menu_auto_resize(menu *pMenu);
// menu_input returns the index of the chosen entry, or -1 if no entry was chosen.
int menu_input(menu* pMenu, SDL_Event *sdlEvent);
const char* menu_current_entry(menu* pMenu);
void menu_render(menu* pMenu);
void menu_destroy(menu* pMenu);


#endif
