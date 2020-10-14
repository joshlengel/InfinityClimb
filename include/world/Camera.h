#pragma once

#include"math/Vec.h"
#include"math/Mat.h"

typedef struct
{
    Vec3 position;
    float pitch, yaw;
} Camera;

void camera_move_forward(Camera *camera, float speed);
void camera_move_right(Camera *camera, float speed);
void camera_move_up(Camera *camera, float speed);

Mat4 camera_view_matrix(const Camera *camera);