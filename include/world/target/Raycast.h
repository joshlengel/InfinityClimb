#pragma once

#include"util/math/Vec.h"
#include"Core.h"

struct _RaycastResult
{
    IC_BOOL collision;
    Vec3 intersection;
    float distance;
};

typedef struct _RaycastResult RaycastResult;

typedef struct _Target Target;

RaycastResult raycast_check_target(const Vec3 *ray_origin, const Vec3 *ray_direction, float ray_length, const Target *target);