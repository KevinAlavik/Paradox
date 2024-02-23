#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>
#include <stdint.h>
#include <system/memory/heap.h>
#include <system/memory/pmm.h>

size_t strlen(const char *str);

long strtol(const char *nptr, char **endptr, int base);

char *strdup(const char *src);

int strncmp(const char *s1, const char *s2, size_t n);

char *strncpy(char *dest, const char *src, size_t n);

char *strcpy(char *dest, const char *src);

int strcmp(const char *s1, const char *s2);

uint64_t strtoul(const char *str, char **endptr, int base);

uint64_t octal_str_to_uint64(const char *str);

int isxdigit(unsigned char c);

int isdigit(unsigned char c);

int isspace(int c);

int islower(int c);
char *strrchr(const char *s, int c);

#endif // STRINGS_H
