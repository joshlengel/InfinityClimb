#pragma once

#include"Core.h"
#include"util/Utils.h"

#include<stdint.h>

typedef struct _Rect Rect;

struct _Level_Data
{
    Loader loader;
    Rect *rects;
    uint32_t num_rects, rect_index;
};

typedef struct _Level Level;