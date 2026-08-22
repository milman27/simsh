#ifndef UTIL_H
#define UTIL_H
#define print(string) do {write(1, (string), _strlen((string)));} while(0)
void * memset(void* ptr, int value, unsigned long len);
int power(int base, int exp);
int stringCmp(const char* first, const char* sec, int len);
int findChar(const char* string, char search);
char* numToStr(char* ret, int value);
int _strlen(const char* str);
char * _strcat(char * buffer, const char * first, const char * second);
#endif
