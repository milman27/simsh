#include "syscall.h"
#include "typedef.h"
#include "util.h"
void * memset(void* ptr, int value, unsigned long len){
    unsigned char chr = (unsigned char)value;
    for(int i = 0; i < len; i++){
        ((unsigned char*)ptr)[i] = chr;
    }
    return ptr;
}
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
int findChar(const char* string, char search){
    int i = 0;
    for(;string[i]; i++){
        if(string[i] == search)
            return i;
    }
    return (search == 0) ? i : -1;
}

char* numToStr(char* ret, int value){
    if(value == 0){
        ret[0] = '0';
        ret[1] = 0;
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


