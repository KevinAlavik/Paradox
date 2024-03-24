#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>

void printfColor(int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b, const char *format, ...);
void nighterm_clear();
int endsWith(const char *str, const char *suffix);
char *get_nighterm_return_string(int return_code);
void decode_and_print_time(uint32_t packed_time);

#endif // UTILITIES_H
