#pragma once

#include"util/math/Vec.h"
#include"util/math/Mat.h"

struct _Camera
{
    Vec3 position;
    float pitch, yaw;

    float fov;
};

typedef struct _Camera Camera;

Mat4 camera_view_matrix(const Camera *camera);