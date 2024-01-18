#include <stdint.h>
#include <stddef.h>
#include <system/memory/memory.h>

int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
int strlen(const char *str);
char *strdup(const char *src);
void strcpy(char dest[], const char source[]);