#include "typedef.h"
#include "syscall.h"
#include "libs.h"
#include "util.h"
#include "arena.h"
#define print(string) do {write(1, (string), _strlen((string)));} while(0)
void handler(int signum){ 
    print("\n");
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
        if(cmdline[i] == '|'){
            cmdline[i] = 0;
            return -1*i;
        }
    }
    args[a + 1] = (char*)0;
    return i;
}
char* getHOME(const char *envp[]){
    int i = 0;
    for(;envp[i];i++){
        if(stringCmp(envp[i], "HOME=",5)){
            break;
        }
    }
    if(!envp[i]){
        return 0;
    }
    return (char *)&envp[i][5];
}
int getPATH(const char *envp[], char *path[]){
    int i = 0;
    for(;envp[i]; i++){
        if(stringCmp(envp[i], "PATH=", 5)){
            break;
        }
    }
    if(!envp[i]){
        return 0;
    }
    int a = 0;
    int temp = 0;
    int j = 5;
    int b = 0;
    while((a = findChar(&envp[i][j], ':')) != -1){
        temp = 0;
        for(; temp < a ; temp++){
            path[b][temp] = envp[i][temp + j];
        }
       j += a + 1; 
       b++;
    }
    a = findChar(&envp[i][j], 0);
    temp = 0;
    for(; temp < a; temp++){
        path[b][temp] = envp[i][temp + j];
    }
    b++;
    return b + 1;

}
char* getPrompt(char* buffer){
    char cwd[50] = {0};
    char promptterm = '>';
    char hashstr[2] = {promptterm, 0};
    getcwd(cwd, 49);
    _strcat(buffer, cwd, hashstr);
    return buffer;
}
__attribute__((weak))
int main(int argc, char* argv[], char* envp[]){
    signal(2, handler);
    int displayPid = 1;
    char * prompt = arenaAlloc(0, 100);
    char * program = arenaAlloc(0, 1000);
    char * path = arenaAlloc(0, 1500);
    memset(path, 0, 1500);
    char *paths[16] = {path + 0, path + 100, path + 200, path + 300, path + 400, path + 500, path + 600, path + 700, path + 800, path + 900, path + 1000, 
        path + 1100, path + 1200, path + 1300, path + 1400, 0};
    int num = getPATH((const char**) envp, paths); 
    for(int i = 0; i < num -1; i++){
        _strcat(paths[i], paths[i], "/");
    }
    while(1){
        print(getPrompt(prompt));
        int red = read(0, program, 99);
        program[red - 1] = 0;
        if(program[0] == 'q'){
            return 0;
        }
        if(program[0] == 0){
            continue;
        }
        char * args[1000] = {0};
        int i = 0;
        char numbers[50];
        int readFromLine = 0;
        for(int parsed = -1; parsed < 0 ;i++){
            parsed = parseInput(&args[i*100], &program[readFromLine]); 
            readFromLine -= parsed;
            readFromLine++;
            //print(numToStr(numbers, readFromLine));
        }
         
        if(program[0] == 'c' && program[1] == 'd'){
            if(chdir(args[1]) == -1){
                if(args[1] != 0){
                    print("Cannot find directory ");
                    print(args[1]);
                    print("\n");
                }else{
                    if(getHOME((const char**)envp) == 0){
                        print("home not found in env");
                    }else{
                        chdir(getHOME((const char **)envp));
                    }
                }
            }
            continue;  
        }
        int pid[10];
        int pipes[2*(i-1)];
        for(int l = 0; l < (i-1); l++){
            pipe(&pipes[2*l]);
        }
        for(int j = 0; j < i; j++){
            if (!(pid[j] = fork())){
                if(i > 1){
                    if(j == 0){
                       dup2(pipes[1], 1);
                       close(pipes[1]);
                    }
                    else if(j == i-1){
                       dup2(pipes[2*(j-1)], 0);
                       close(pipes[2*(j-1)]);
                    }else{
                       dup2(pipes[2*(j-1)],0);
                       close(pipes[2*(j-1)]);
                       dup2(pipes[2*j + 1],1);
                       close(pipes[2*j+1]);
                    }
                }
                char pathex[100] = {0}; int ret = 0; 
                for(int q = 0; q < num; q++){
                    _strcat(pathex, paths[q], args[j*100]);
                    ret = exec(pathex, (const char* const *)&(args[j*100]), (const char* const *)envp);
                }
                if(ret == 2){
                    print("File not found: ");
                    print(args[j*100]); 
                    print("\n");
                    if(i >1){
                        close(pipes[2*j+1]);
                        close(pipes[2*j]);
                    }
                }
                return ret;
            }else{
                if(i > 1){
                    if(j == 0){
                       close(pipes[1]);
                    }
                    else if(j == i-1){
                       close(pipes[2*(j-1)]);
                    }else{
                       close(pipes[2*(j-1)]);
                       close(pipes[2*j+1]);
                    }
                }
                if(displayPid){
                    print(numToStr(numbers, pid[j]));
                    print("\n");
                }
            }
        }
            wait4(pid[i-1], 0, 0, 0); 
    }
}

