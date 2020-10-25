#pragma once

#include"Core.h"

#include<stdint.h>
#include<stdio.h>

#ifdef IC_DEBUG
    #define log_assert(expression, ...) if (!(expression)) { log_trace(__VA_ARGS__); asm("int $3"); }
#else
    #define log_assert(expression, ...)
#endif // IC_DEBUG

void log_trace(const char *fmt, ...);

uint32_t get_log_size();
void get_log(char *dest, uint32_t size);

void flush_log();
void dump_log(FILE *file);