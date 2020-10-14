#pragma once

#include"Core.h"

#include<stdint.h>

typedef struct Window_Data Window_Data;

typedef struct
{
    uint32_t width, height;
    const char *title;

    const Window_Data *data;
} Window;

IC_ERROR_CODE window_create(Window *dest);
void window_destroy(const Window *window);

void window_show(const Window *window);
IC_BOOL window_should_close(const Window *window);
void window_swap_buffers(const Window *window);
void window_poll_events();