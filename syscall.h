extern int errnu;
extern int syscall( int unsigned number, ...);
extern int write(unsigned int fd, const char* string, unsigned int length);
extern int read(unsigned int fd, char* buf, int length);
extern int exec(const char* filename, const char* const *argv, const char *const *envp);
extern int fork(void);
extern char* getcwd(char *buf, unsigned long size);
extern int chdir(const char*);
extern int wait4(int, int*, int, struct rusage*);
