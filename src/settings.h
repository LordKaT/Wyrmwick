#ifndef __SETTINGS_H__
#define __SETTINGS_H__

settings* settings_init();

// the userdata pointer is available to function handlers as a light userdata upvalue 1.
void settings_add_func(settings *s, const char *name, lua_CFunction func, void *userdata);

void settings_add_writer(settings *s, settings_writer_func swriter, void *userdata);

// returns 0 on success, 1 on failure
int settings_load(settings *s, const char* path);
int settings_save(settings *s, const char* path);

void settings_destroy(settings *s);

#endif
