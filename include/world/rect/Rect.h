#pragma once

#include"Core.h"
#include"math/Vec.h"

typedef struct Rect_Data Rect_Data;

typedef struct
{
    Vec3 position;
    Vec3 scale;

    const Rect_Data *data;
} Rect;

IC_ERROR_CODE rect_create(Rect *dest);
void rect_destroy(const Rect *rect);

void rect_render(const Rect *rect);