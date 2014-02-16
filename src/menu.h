#ifndef __MENU_H__
#define __MENU_H__

menu* menu_init(const char *cMenuTitle, int xPos, int yPos);
void menu_add_entry(menu *pMenu, const char *fmt, ...);
// menu_input returns the index of the chosen entry, or -1 if no entry was chosen.
int menu_input(menu* pMenu, SDL_Event *sdlEvent);
void menu_render(menu* pMenu);
void menu_destroy(menu* pMenu);


#endif
