#include <system/utilities/utilities.h>
#include <printf.h>
#include <stdarg.h>

void printfColor(int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b, const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (const char* letter = format; *letter != '\0'; ++letter) {
        nighterm_set_char_fg(fg_r, fg_g, fg_b);
        nighterm_set_char_bg(bg_r, bg_g, bg_b);
        printf("%c", *letter);
        nighterm_set_char_fg(255, 255, 255);
        nighterm_set_char_bg(0, 0, 0);
    }

    va_end(args);
}
