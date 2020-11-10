#pragma once

#include"Core.h"

typedef struct _Color Color;

typedef struct _Context_Data Context_Data;

struct _Context
{
    IC_BOOL clear_color, clear_depth;
    const Color *background_color;

    IC_BOOL cull, cull_front;
    IC_BOOL depth_test;
    IC_BOOL blending;

    Context_Data *data;
};

typedef struct _Context Context;

IC_ERROR_CODE context_create(Context *dest);
void context_destroy(const Context *context);
void context_update(const Context *context);
void context_clear(const Context *context);