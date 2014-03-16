#include "include.h"

file_dialog* file_dialog_init(int x, int y, const char *path, const char *root) {
	file_dialog *fd = (file_dialog*) malloc(sizeof(file_dialog));
	fd->m_menu = menu_init(x, y);
	fd->m_menu->m_iWidth = 600;
	fd->m_menu->m_iHeight = 700;
	fd->m_sRoot = strdup(root);
	fd->m_sPath = strdup(path);
	
	if (! file_dialog_reload(fd)) {
		file_dialog_destroy(fd);
		return nullptr;
	}
	
	return fd;
}

bool file_dialog_reload(file_dialog *fd) {
	const char *entry;
	bool onRoot = (strcmp(fd->m_sPath, fd->m_sRoot) == 0);
	sys_dir *sd = sys_dir_open(fd->m_sPath);
	if (! sd) { return false; }
	for (;;) {
		entry = sys_dir_next(sd);
		if (! entry) { break; }
		if (strcmp(entry, ".") == 0) { continue; }
		if (onRoot && strcmp(entry, "..")==0) { continue; }
		menu_add_entry(fd->m_menu, "%s", entry);
	}
	sys_dir_close(sd);
	util_sort_menu(fd->m_menu);
	return true;
}

bool file_dialog_input(file_dialog *fd, SDL_Event *sdlEvent) {
}

void file_dialog_get(file_dialog *fd, char **dir, char **file) {
	if (dir) { *dir = strdup(fd->m_sPath); }
	*file = strdup(menu_current_entry(fd->m_menu));
}

void file_dialog_render(file_dialog *fd) {
	menu_render(fd->m_menu);
}

void file_dialog_destroy(file_dialog *fd) {
	menu_destroy(fd->m_menu);
	free(fd->m_sRoot);
	free(fd->m_sPath);
	free(fd);
}

