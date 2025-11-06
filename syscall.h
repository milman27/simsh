#ifndef SYSCALL
#define SYSCALL
#include "typedef.h"
 static int errnu;
 uint64_t syscall(int unsigned number, ...);
 int write(unsigned int fd, const char* string, unsigned int length);
 int read(unsigned int fd, char* buf, int length);
 int exec(const char* filename, const char* const *argv, const char *const *envp);
 int fork(void);
 char* getcwd(char *buf, unsigned long size);
 int chdir(const char*);
 int wait4(int pid, int* wstatus, int options, void* rusage);
 void* AAlloc(size_t size);
 void _exit(int);
#endif
