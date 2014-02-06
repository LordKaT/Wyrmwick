#ifndef __MENU_H__
#define __MENU_H__

void menu_init();
void menu_create(char *cMenuTitle, int iXPos, int iYPos, void (*vFunc)(int iSelection));
void menu_add(char *cFmt, ...);
void menu_input(SDL_Event *sdlEvent);
void menu_render();
void menu_close();
bool menu_is_open();
void menu_destroy();

#endif
