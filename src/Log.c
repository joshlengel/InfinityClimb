#define _CRT_SECURE_NO_WARNINGS
#pragma warning(push)
#pragma warning(disable: 26451)

#include"Log.h"

#include"util/Vector.h"

#include<stdio.h>
#include<stdarg.h>
#include<string.h>
#include<time.h>

static Vector log_str = {.elem_size=sizeof(char), .init_capacity=1000};

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

const char NULL_TERMINATOR = '\0';
const char NEWLINE = '\n';

IC_ERROR_CODE log_init()
{
    IC_ERROR_CODE ec = vector_create(&log_str);
    if (ec != IC_NO_ERROR)
    {
        return ec;
    }

    vector_add(&log_str, &NULL_TERMINATOR);

    return IC_NO_ERROR;
}

void log_terminate()
{
    vector_destroy(&log_str);
}

void __log_impl(const char *fmt, va_list args)
{
    uint32_t rem_size = log_str.capacity - log_str.size + 1;
    
    int written = vsnprintf((char*)log_str.arr + log_str.size - 1, rem_size, fmt, args);

    uint32_t size = log_str.size + written;
        
    while (written >= (signed)rem_size)
    {
        vector_add(&log_str, &NULL_TERMINATOR); // This will be overwritten anyway
        rem_size = log_str.capacity - log_str.size + 1;
        written = vsnprintf((char*)log_str.arr + log_str.size, rem_size, fmt, args);
        size = log_str.size + written;
    }

    log_str.size = size;
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
    struct tm local_time = *localtime(&t);
    const char *prefix_fmt = "[%02d %s %d - %02d:%02d:%02d] ";
    
    __log_vargs_impl(prefix_fmt, local_time.tm_mday, months[local_time.tm_mon], local_time.tm_year + 1900, local_time.tm_hour, local_time.tm_min, local_time.tm_sec);

    va_list args;
    va_start(args, fmt);

    __log_impl(fmt, args);

    log_str.size -= 1;

    vector_add(&log_str, &NEWLINE);
    vector_add(&log_str, &NULL_TERMINATOR);
}

uint32_t get_log_size()
{
    return log_str.size - 1;
}

void get_log(char *dest, uint32_t size)
{
    if (size < log_str.size)
    {
        memcpy(dest, log_str.arr, size);
    }
    else
    {
        memcpy(dest, log_str.arr, log_str.size);
    }
}

void flush_log()
{
    log_str.size = 1;
    vector_put(&log_str, &NULL_TERMINATOR, 0);
}

void dump_log(FILE *file)
{
    fputs((char*)log_str.arr, file);
}

#pragma warning(pop)