#include "syscall.h"
#include "util.h"
#include "libs.h"
int signal(int signum, void(*funcptr)(int)){
    struct sigaction sig;
    sig.sighandle = funcptr;
    sig.sa_flags = SA_RESTORER;
    sig.sa_restorer = sigreturn;
    memset(&sig.sa_mask, 0, 8);
    int ret = sigaction(2, &sig, 0, 8);
    return ret; 
}
