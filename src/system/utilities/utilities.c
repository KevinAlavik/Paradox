#include <system/utilities/utilities.h>
#include <nighterm/backends/vga.h>
#include <nighterm/nighterm.h>
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

void nighterm_clear() {
    nighterm_flush(0, 0, 0);
    term.cx = 0;
    term.cy = 0;
}

char* get_nighterm_return_string(int return_code) {
    switch (return_code) {
        case NIGHTERM_NO_FONT_SUPPLIED:
            return "NIGHTERM_NO_FONT_SUPPLIED";
        case NIGHTERM_FONT_INVALID:
            return "NIGHTERM_FONT_INVALID";
        case NIGHTERM_INVALID_FRAMEBUFFER_ADDRESS:
            return "NIGHTERM_INVALID_FRAMEBUFFER_ADDRESS";
        case NIGHTERM_INVALID_FRAMEBUFFER_SIZE:
            return "NIGHTERM_INVALID_FRAMEBUFFER_SIZE";
        case NIGHTERM_INVALID_FRAMEBUFFER_PITCH:
            return "NIGHTERM_INVALID_FRAMEBUFFER_PITCH";
        case NIGHTERM_INVALID_FRAMEBUFFER_BPP:
            return "NIGHTERM_INVALID_FRAMEBUFFER_BPP";
        case NIGHTERM_MALLOC_IS_NULL:
            return "NIGHTERM_MALLOC_IS_NULL";
        case NIGHTERM_SUCCESS:
            return "NIGHTERM_SUCCESS";
        default:
            return "Unknown return code";
    }
}