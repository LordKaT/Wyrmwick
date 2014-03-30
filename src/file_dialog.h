// FIXME: Do we ever want the user to go out of the game's main directory?
file_dialog* file_dialog_init(int x, int y, const char *path, const char *root);

bool file_dialog_reload(file_dialog *fd);

// file_dialog_input returns true if a file has been selected.
bool file_dialog_input(file_dialog *fd, SDL_Event *sdlEvent);

// file_dialog_get puts the base path and file name of the selected file in dir and path.
// dir may be NULL. Both returned pointers should be freed with free().
void file_dialog_get(file_dialog *fd, char **dir, char **file);
void file_dialog_render(file_dialog *fd);

void file_dialog_destroy(file_dialog *fd);
