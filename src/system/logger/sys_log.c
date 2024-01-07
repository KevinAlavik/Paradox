#include <system/logger/sys_log.h>
#include <system/utilities/utilities.h>
#include <printf.h>
#include <stdarg.h>

void log(LogLevel level, const char* format, ...) {
    va_list args;
    va_start(args, format);

    const char* level_str;
    int r, g, b;
    switch (level) {
        case OK:
            level_str = "Ok";
            r = 0; g = 255; b = 0;
            break;
        case DEBUG:
            level_str = "Debug";
            r = g = b = 150;
            break;
        case INFO:
            level_str = "Info";
            r = 0; g = 0; b = 255;
            break;
        case WARNING:
            level_str = "Warning";
            r = 255; g = 165; b = 0;
            break;
        case ERROR:
            level_str = "Error";
            r = 255; g = 0; b = 0;
            break;
        default:
            level_str = "UNKNOWN";
            r = g = b = 125;
            break;
    }

    printf("[ ");

    printfColor(r, g, b, 0, 0, 0, level_str);

    printf(" ] ");

    vprintf(format, args);
    printf("\n");

    va_end(args);
}
