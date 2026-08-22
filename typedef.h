#ifndef TYPEDEF_M
#define TYPEDEF_M
#define SA_RESTORER 0x04000000
#define FALSE 0
#define TRUE 1
typedef char unsigned uint8_t;
typedef short unsigned uint16_t;
typedef  long long unsigned uint64_t;
typedef uint64_t size_t;
typedef struct sigset_t{
    uint64_t data[2];
}sigset_t;
struct sigaction{
    void (*sighandle)(int);
    uint64_t sa_flags;
    void (*sa_restorer)(void);
    sigset_t sa_mask;
};
#endif
