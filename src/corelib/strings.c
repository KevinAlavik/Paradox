#include "strings.h"
#include <stddef.h>
#include <system/memory/memory.h>

size_t strlen(const char *str) {
    size_t n = 0;
    while (str[n] != '\0') {
        n++;
    }
    return n;
}

char *strdup(const char *src) {
    size_t length = strlen(src);
    char *dst = malloc(length + 1);
    if (dst != NULL) {
        strncpy(dst, src, length + 1);
    }
    return dst;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
        --n;
    }
    if (n == 0) {
        return 0;
    } else {
        return (*(unsigned char *)s1 - *(unsigned char *)s2);
    }
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

char *strcpy(char *dest, const char *src) {
    char *start = dest;
    while ((*dest++ = *src++) != '\0');
    return start;
}
