#ifndef SYS_LOG_H
#define SYS_LOG_H

typedef enum {
    OK,
    DEBUG,
    INFO,
    WARNING,
    ERROR
} LogLevel;

void log_message(LogLevel level, const char* format, ...);

#endif // SYS_LOG_H