#pragma once

#include"Core.h"

typedef struct _Crosshairs_Data Crosshairs_Data;

struct _Crosshairs
{
    const char *texture_path;
    float scale;

    const Crosshairs_Data *data;
};

typedef struct _Crosshairs Crosshairs;

typedef struct _Window Window;

IC_ERROR_CODE crosshairs_create(Crosshairs *dest);
void crosshairs_destroy(const Crosshairs *crosshairs);

void crosshairs_render(const Crosshairs *crosshairs, const Window *window);