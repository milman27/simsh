#ifndef TYPEDEF_M
#define TYPEDEF_M
typedef char unsigned uint8_t;
typedef short unsigned uint16_t;
typedef  long long unsigned uint64_t;
typedef uint64_t size_t;
typedef struct sigset_t{
    uint8_t data[128];
}sigset_t;
struct sigaction{
    void (*sighandle)(int);
    uint64_t sa_flags;
    void (*sa_restorer)(void);
    sigset_t sa_mask;
};
#endif
