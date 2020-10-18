#pragma once

#include"util/math/Vec.h"
#include"util/math/Mat.h"

typedef struct _Mesh Mesh;
typedef struct _Collidable Collidable;

struct _Model
{
    const Mesh *mesh;
    const Collidable *collidable;

    Vec3 rotation_axis;
    float rotation;

    Vec3 scale;

    Vec3 position;
    Vec3 velocity, temp_velocity;
    Vec3 acceleration, temp_acceleration;
};

typedef struct _Model Model;

Mat4 model_transform_matrix(const Model *model);
void model_render(const Model *model);
void model_update(Model *model, float dt);