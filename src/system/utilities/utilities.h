#ifndef UTILITIES_H
#define UTILITIES_H

void printfColor(int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b, const char* format, ...);
void nighterm_clear();
int endsWith(const char *str, const char *suffix);
char* get_nighterm_return_string(int return_code); 
int tolower(int c);

#endif // UTILITIES_H
