#pragma once

#include"util/math/Vec.h"

struct _AABB
{
    Vec3 center;
    Vec3 extent;
};

typedef struct _AABB AABB;

struct _Collision_Result
{
    Vec3 obj1_displacement;
    Vec3 obj1_vel, obj1_accum_vel;

    double time_simulated;
};

typedef struct _Collision_Result Collision_Result;

Collision_Result physics_collide(const AABB *obj1, const Vec3 *vel1, const Vec3 *accum_vel1, const AABB *obj2, float dt);