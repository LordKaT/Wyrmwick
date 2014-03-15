#include <sys/types.h>
#include <dirent.h>

typedef struct sys_dir {
	DIR *dir;
	dirent entry;
} sys_dir;

void sys_abort_at(const char* file, int line) {
	printf("Aborted at %s:%d\n", file, line);
	abort();
}

sys_dir* sys_dir_open(const char *path) {
	sys_dir *dir;
	dir = (sys_dir*) malloc(sizeof(sys_dir));
	if (! dir) {
		return nullptr;
	}
	dir->dir = opendir(path);
	if (! dir->dir) {
		free(dir);
		return nullptr;
	}
	return dir;
}

const char* sys_dir_next(sys_dir *pdir) {
	int err;
	dirent* not_end;
	err = readdir_r(pdir->dir, &(pdir->entry), &not_end);
	if (err || (!not_end)) {
		return nullptr;
	}
	
	return pdir->entry.d_name;
}

void sys_dir_close(sys_dir *pdir) {
	closedir(pdir->dir);
	free(pdir);
}
