#include"Log.h"

#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<assert.h>

static char *log_str;
static uint32_t log_size, log_index;

const uint32_t LOG_RESIZE = 1000;

const char *months[] =
{
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Okt",
    "Nov",
    "Dec"
};

void log_init()
{
    assert(LOG_RESIZE > 1);

    log_str = malloc(sizeof(char) * LOG_RESIZE);
    log_str[0] = '\0';
    log_size = LOG_RESIZE;
    log_index = 0;
}

void log_terminate()
{
    free(log_str);
    log_size = 0;
    log_index = 0;
}

void resize_log()
{
    char *temp = log_str;
    log_str = realloc(log_str, sizeof(char) * (log_size += LOG_RESIZE));

    if (!log_str)
    {
        free(temp);
        return;
    }
}

void __log_impl(const char *fmt, va_list args)
{
    uint32_t rem_size = log_size - log_index;
    
    int written = vsnprintf(log_str + log_index, rem_size, fmt, args);

    while (written >= (signed)rem_size)
    {
        resize_log();
        rem_size = log_size - log_index;
        written = vsnprintf(log_str + log_index, rem_size, fmt, args);
    }

    log_index += written;
}

void __log_vargs_impl(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    __log_impl(fmt, args);
}

void log_trace(const char *fmt, ...)
{
    // Get time and log it
    time_t t = time(NULL);
    struct tm local_time;
    localtime_s(&local_time, &t);
    const char *prefix_fmt = "[%02d %s %d - %02d:%02d:%02d] ";
    
    __log_vargs_impl(prefix_fmt, local_time.tm_mday, months[local_time.tm_mon], local_time.tm_year + 1900, local_time.tm_hour, local_time.tm_min, local_time.tm_sec);

    va_list args;
    va_start(args, fmt);

    __log_impl(fmt, args);

    if ((int32_t)log_index >= (int32_t)log_size - 1)
    {
        resize_log();
    }

    log_str[log_index++] = '\n';
    log_str[log_index] = '\0';
}

uint32_t get_log_size()
{
    return log_index;
}

void get_log(char *dest, uint32_t size)
{
    if (size < log_index)
    {
        memcpy(dest, log_str, size);
    }
    else
    {
        memcpy(dest, log_str, log_index);
    }
}

void flush_log()
{
    log_index = 0;
}

void dump_log(FILE *file)
{
    fputs(log_str, file);
}