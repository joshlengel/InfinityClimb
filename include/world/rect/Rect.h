#pragma once

#include"Core.h"
#include"util/math/Vec.h"
#include"util/math/Mat.h"
#include"world/Physics.h"

typedef struct _Rect_Data Rect_Data;

struct _Rect
{
    AABB aabb;

    const Rect_Data *data;
};

typedef struct _Rect Rect;

IC_ERROR_CODE rect_create(Rect *dest);
void rect_destroy(const Rect *rect);
void rect_render(const Rect *rect);
Mat4 rect_transform_matrix(const Rect *rect);