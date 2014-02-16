#ifndef __SYS_H__
#define __SYS_H__

// Call sys_abort() if something horrible happened and the program absolutely cannot continue.
#define sys_abort() sys_abort_at(__FILE__, __LINE__)

void sys_abort_at(const char* file, const int line);

#endif
