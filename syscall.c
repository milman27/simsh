#define FALSE 0
#define TRUE 1
#define PROT_READ 1 
#define PROT_WRITE 2
#define MAP_ANON 32
#define MAP_PRIVATE 2
#include "syscall.h"
#include "typedef.h"
static int errnu = 0;
uint64_t syscall( int unsigned number, ...){
    uint64_t retrn;
    __asm__ __volatile__(
            "mov %1, %%eax\n\t"
            "mov -168(%%rbp), %%rdi\n\t"
            "mov -160(%%rbp), %%rsi\n\t"
            "mov -152(%%rbp), %%rdx\n\t"
            "mov -144(%%rbp), %%r10\n\t"
            "mov -136(%%rbp), %%r8\n\t"
            "mov 16(%%rbp), %%r9\n\t"
            "syscall\n\t"
            "mov %%rax, %0"
            : "=r" (retrn)
            : "r" (number)
            : "%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9"
            );
    return retrn;
}
int fork(void){
    return syscall(0x39);
}
int exec(const char* filename, const char* const *argv, const char *const *envp){
    int ret = syscall(0x3b, filename, argv, envp);
    return (ret < 0) ? -ret : 0;
}
int read(unsigned int fd, char* buf, int length){
    return syscall(0, fd, buf, length); 
}
int write(unsigned int fd, const char* string, unsigned int length){
    return syscall(1, fd, string, length);
}
char * getcwd(char* buf, unsigned long size){
    return (char *)syscall(0x4f, buf, size); 
}
int chdir(const char* path){
   int ret = 
     syscall(0x50, path);
   errnu = (ret < 0) ? -ret: 0;
   return (ret < 0) ? -1: 0;
}
int wait4(int pid, int* wstatus, int options, void* rusage){
    return syscall(0x3d, pid, wstatus, options, rusage);
}
void* _mmap(void *addr, uint64_t length, int prot, int flags, int fd, int offset){
    return (void*)syscall(0xc0, addr, length, prot, flags, fd, offset);
}
void* AAlloc(size_t size){
   return _mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);  
}

