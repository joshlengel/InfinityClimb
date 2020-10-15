#pragma once

#include"window/Input.h"
#include"math/Vec.h"
#include"math/Mat.h"

struct _Camera
{
    Vec3 position;
    float pitch, yaw;
};

typedef struct _Camera Camera;

Mat4 camera_view_matrix(const Camera *camera);