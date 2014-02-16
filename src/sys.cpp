#include "include.h"

void sys_abort_at(const char* file, const int line) {
	printf("Aborted at %s:%d\n", file, line);
#ifdef _MSC_VER
	// FIXME: Windows can do some stupid crap on abort, as it is wont to do.
	abort();
#else
	abort();
#endif
}
