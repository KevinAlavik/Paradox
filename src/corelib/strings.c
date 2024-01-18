#include "strings.h"

int strncmp(const char *s1, const char *s2, size_t n)
{
	while (n && *s1 && (*s1 == *s2)) {
		++s1;
		++s2;
		--n;
	}
	if (n == 0) {
		return 0;
	} else {
		return (*(uint8_t *)s1 - *(uint8_t *)s2);
	}
}

char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;
	for (i = 0; i < n; i++) {
		dest[i] = src[i];
		if (src[i] == '\0') {
			break;
		}
	}
	return dest + i;
}

int strlen(const char *str)
{
	int n = 0;
	while (*str != '\0') {
		n++;
		str++;
	}

	return n;
}

char *strdup(const char *src) {
    char *dst = malloc(strlen (src) + 1);
    if (dst == NULL) return NULL; 
    strcpy(dst, src);                     
    return dst;                            
}

void strcpy(char dest[], const char source[])
{
    int i = 0;
    while (1)
    {
        dest[i] = source[i];

        if (dest[i] == '\0')
        {
            break;
        }

        i++;
    } 
}