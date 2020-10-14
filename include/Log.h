#pragma once

#include<stdint.h>
#include<stdio.h>

void log_trace(const char *fmt, ...);

uint32_t get_log_size();
void get_log(char *dest, uint32_t size);

void flush_log();
void dump_log(FILE *file);