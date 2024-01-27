#include "strings.h"
#include <stddef.h>
#include <system/memory/memory.h>
#include <system/utilities/utilities.h>

size_t strlen(const char *str) {
    size_t n = 0;
    while (str[n] != '\0') {
        n++;
    }
    return n;
}

long strtol(const char *nptr, char **endptr, int base) {
    long result = 0;
    int sign = 1;

    // Skip leading whitespaces
    while (*nptr == ' ' || (*nptr >= '\t' && *nptr <= '\r')) {
        nptr++;
    }

    // Check for the sign
    if (*nptr == '-' || *nptr == '+') {
        sign = (*nptr++ == '-') ? -1 : 1;
    }

    // Determine the base if not provided
    if (base == 0) {
        base = (*nptr == '0' && (nptr[1] == 'x' || nptr[1] == 'X')) ? 16 : 10;
    }

    // Parse the digits
    while ((*nptr >= '0' && *nptr <= '9') ||
           (base == 16 && ((*nptr >= 'a' && *nptr <= 'f') || (*nptr >= 'A' && *nptr <= 'F')))) {
        int digit = *nptr - '0';
        if (base == 16 && *nptr >= 'a') {
            digit = (*nptr - 'a') + 10;
        } else if (base == 16 && *nptr >= 'A') {
            digit = (*nptr - 'A') + 10;
        }

        result = result * base + digit;
        nptr++;
    }

    // Set endptr if provided
    if (endptr != NULL) {
        *endptr = (char *)nptr;
    }

    return result * sign;
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

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int diff = tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        if (diff != 0) {
            return diff;
        }
        s1++;
        s2++;
    }

    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}