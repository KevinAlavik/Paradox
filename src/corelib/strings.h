#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

size_t strlen(const char *str);
char *strdup(const char *src);
int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);

#endif /* STRINGS_H */
