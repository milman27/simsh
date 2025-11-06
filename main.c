#define FALSE 0
#define TRUE 1
#include "typedef.h"
#include "syscall.h"
#define print(string) do {write(1, (string), _strlen((string)));} while(0)
int power(int base, int exp){
    int i = 0;
    int result = 1;
    for(; i < exp; i++){
        result *= base;
    }
    return result;
}

int stringCmp(const char* first, const char* sec, int len){
    if(first == 0 || sec == 0)
        return 0;
    
    int i = 0;
    for(; i < len; i++){
        if(first[i] == '\0' || sec[i] == '\0')
            return first[i] == sec[i];
        if(first[i] != sec[i])
            return 0;
    }
    return 1;
}

char* numToStr(char* ret, int value){
    if(value == 0){
        ret[0] = '0';
        return ret;
    }
        
    int ptr = 0;
    int top = FALSE;
    int i = 9;
    for(; i >= 0; i--){
        int powe = power(10, i); 
        if(powe > value && !top ){
            continue;
        }
        top = TRUE;
        ret[ptr++] = value/(int)powe + '0';
        value = value - ((value/(int)powe) * (int)powe);
    }
    ret[ptr] = '\0';

    return ret;
}

int _strlen(const char* str){
    int i = 0;
    while(str[i]){
        i++;
    }
    return i;
}

int parseInput(char* args[], char * cmdline ){ 
    int i = 0;
    int a = 0;
    char * temp = cmdline;
    for(i = 0; cmdline[i] != '\0'; i++){
        while(cmdline[i] == ' ' || cmdline[i]  == '\t' || cmdline[i] == '\n' || cmdline[i] == '\r'){
            cmdline[i] = '\0';
            temp = &cmdline[++i];
        }
        if((i == 0 || cmdline[i - 1] == '\0') && cmdline[i] != '\0'){
            args[a] = temp;
            a++;
        }
    }
    args[a + 1] = (char*)0;
#if 0 
    i = 0;
    while(args[i] != 0){
        char * str = numToStr(i);
        print(str);
        print(":");
        print(args[i]);
        print("\n");
        i++;
    }
#endif
    return a;
}
int findChar(const char* string, char search){
    int i = 0;
    for(;string[i]; i++){
        if(string[i] == search)
            return i;
    }
    return (search == 0) ? i : -1;
}
int getPATH(const char *argv[], char *path[]){
    int i = 0;
    for(;argv[i]; i++){
        if(stringCmp(argv[i], "PATH=", 5)){
            break;
        }
    }
    if(!argv[i]){
        return 0;
    }
    int a = 0;
    int temp = 0;
    int j = 5;
    char buf[100];
    int b = 0;
    while((a = findChar(&argv[i][j], ':')) != -1){
        temp = 0;
        for(; temp < a ; temp++){
            path[b][temp] = argv[i][temp + j];
        }
       j += a + 1; 
       b++;
    }
    a = findChar(&argv[i][j], 0);
    temp = 0;
    for(; temp < a; temp++){
        path[b][temp] = argv[i][temp + j];
    }
    b++;
    return b + 1;

}
char * _strcat(char * buffer, const char * first, const char * second){
    int len = _strlen(first);
    int len2 = _strlen(second);
    for(int i = 0; i < _strlen(first); i++){
        buffer[i] = first[i]; 
    }
    for (int i = 0; i < _strlen(second); i++){
        buffer[i + len] = second[i]; 
    }
    buffer[len + len2] = 0;
    return buffer;
}
int main(int argc, char* argv[], char* envp[]){
    char program[100] = {0};
    char cwd[50] = {0};
    char path[15][100] = {0};     
    char *paths[16] = {path[0], path[1], path[2], path[3], path[4], path[5], path[6], path[7], path[8], path[9], path[10], path[11], path[12], path[13], path[14], 0};
    int num = getPATH((const char**) envp, paths); 
    for(int i = 0; i < num -1; i++){
        _strcat(paths[i], paths[i], "/");
    }
    while(1){
        getcwd(cwd, 49);
        print(cwd);
        print("#");
        int red = read(0, program, 90);
        program[red - 1] = 0;
        if(program[0] == 'q'){
            return 0;
        }
        if(program[0] == 0){
            continue;
        }
        char * args[100] = {0};
        parseInput(args, program);
        
        if(program[0] == 'c' && program[1] == 'd'){
            if(chdir(args[1]) == -1){
                print("Cannot find directory ");
                print(args[1]);
                print("\n");
            }
            continue;  
        }
        int pid;
        if (!(pid = fork())){
            char pathex[100] = {0};
            int ret = 0; 
            for(int i = 0; i < num; i++){
                _strcat(pathex, paths[i], program);
                ret = exec(pathex, (const char* const *)args, (const char* const *)envp);
            }
            if(ret == 2){
                print("File not found: ");
                print(program); 
                print("\n");
            }
            char buffer[20];
            char* number = numToStr(buffer, ret);

            return ret;
        }
        wait4(pid, 0, 0, 0);
    }
}
int _start(){
    int argc;
    char ** argv;
    char ** envp;
    __asm__ ("mov 0x8(%%rbp),%0\n\tmovq %%rbp, %1\n\tadd $0x10, %1"
                            : "=r" (argc), "=r" (argv) 
    );
    envp = (argv + argc) + 1;
   _exit(main(argc, argv, envp)); 
}
