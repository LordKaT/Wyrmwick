#include <windows.h>

#include "../include.h"

typedef struct sys_dir {
	HANDLE dir;
	WIN32_FIND_DATA entry;
	bool first;
} sys_dir;

void sys_abort_at(const char* file, int line) {
	printf("Aborted at %s:%d\n", file, line);
	// FIXME: Windows can do some stupid crap on abort, as it is wont to do.
	abort();
}

sys_dir* sys_dir_open(const char *path) {
	sys_dir *dir = (sys_dir*) malloc(sizeof(sys_dir));
	if (!dir) {
		return nullptr;
	}
	
	char *spath = (char *) malloc(strlen(path)+strlen("\\*.*"));
	if (!spath) {
		free(dir);
		return nullptr;
	}
	
	// Replace slashes with backslashes.
	int i;
	int len = strlen(path);
	for (i = 0; i < len; i++) {
		if (path[i] == '\0') { break; }
		if (path[i] == '/') {
			spath[i] = '\\';
			continue;
		}
		spath[i] = path[i];
	}
	strcpy(spath + i, "\\*.*");
	
	// I'm pretty sure LPCTSTR is char*, Microsoft programmers are just paid per character.
	// That, or they do too much drugs and thought they were coding in Pascal.
	dir->dir = FindFirstFile(spath, &(dir->entry));
	if (dir->dir == INVALID_HANDLE_VALUE) {
		free(dir);
		free(spath);
		return nullptr;
	}
	dir->first = true;
	return dir;
}

const char* sys_dir_next(sys_dir *pdir) {
	if (pdir->first) {
		pdir->first = false;
		return pdir->entry.cFileName;
	}
	int ok = FindNextFile(pdir->dir, &(pdir->entry));
	if (!ok) {
		return nullptr;
	}
	return pdir->entry.cFileName;
}

void sys_dir_close(sys_dir *pdir) {
	FindClose(pdir->dir);
	free(pdir);
	return;
}
