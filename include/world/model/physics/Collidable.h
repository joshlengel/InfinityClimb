#pragma once

#include"util/math/Vec.h"

struct _AABB
{
    Vec3 extent;
};

struct _Sphere
{
    float radius;
};

struct _Capsule
{
    float radius;
    float body_height; // exclude caps (total height - radius * 2)
};

enum _Collidable
{
    IC_COLLIDABLE_AABB,
    IC_COLLIDABLE_CAPSULE
};

struct _Collision_Result
{
    /*float time_simulated;*/
    Vec3 res_velocity;
    Vec3 displacement;
};

typedef struct _AABB AABB;
typedef struct _Sphere Sphere;
typedef struct _Capsule Capsule;
typedef enum _Collidable Collidable;
typedef struct _Collision_Result Collision_Result;

typedef struct _Model Model;
typedef struct _Player Player;

Collision_Result collide_sphere_with_static(const Sphere *sphere, const Model *model, const Player *player, float dt);
Collision_Result collide_capsule_with_static(const Capsule *capsule, const Model *model, const Player *player, float dt);