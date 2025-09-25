
#ifndef XTB_LOG_H_
#define XTB_LOG_H_

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

typedef enum {
    log_trace,
    log_debug,
    log_info,
    log_warn,
    log_error,
    log_fatal,
} log_level_t;

static const char* log_level_str[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

static log_level_t threshold_level = 0;
static FILE* out = {0};

void log_set_output(FILE* file);
void log_write_level_color(log_level_t level);
void log_set_threshold(log_level_t level);
void log_write_time(void);
void log_init_output(void);

#define LOG(level, ...) \
    do { \
        log_init_output(); \
        if ((level) >= threshold_level) { \
            log_write_level_color(level); \
            log_write_time(); \
	    fprintf(out,"[%s:%d] [%s] ",__FILE__,__LINE__,log_level_str[level]);\
            fprintf(out, __VA_ARGS__); \
            fprintf(out,"\x1b[0m\n"); \
        } \
        if ((level) == log_fatal) exit(1); \
    } while(0)


#ifdef XTB_LOG_IMPEMENTATION

void log_init_output
(void)
{
    if (!out) out = stdout;
}

void log_set_output
(FILE* file)
{
    out = file;
}

void log_set_threshold
(log_level_t level)
{
    threshold_level = level;
}

void log_write_level_color
(log_level_t level)
{
    switch(level)
    {
	case log_trace: fprintf(out, "\x1b[37m"); break; // White
        case log_debug: fprintf(out, "\x1b[36m"); break; // Cyan
        case log_info:  fprintf(out, "\x1b[32m"); break; // Green
        case log_warn:  fprintf(out, "\x1b[33m"); break; // Yellow
        case log_error: fprintf(out, "\x1b[31m"); break; // Red
        case log_fatal: fprintf(out, "\x1b[41m\x1b[37m"); break; // White on Red background
	default: printf("Unknown log level");exit(1);
    }
}

void log_write_time
(void)
{
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char* ctime = asctime(local_time);
    ctime[strlen(ctime) - 1] = 0;
    fprintf(out,"[%s] ",ctime);
}

#endif // XTB_LOG_IMPEMENTATION

#endif // XTB_LOG_H_
