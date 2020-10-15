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

void camera_move_forward(Camera *camera, float speed);
void camera_move_right(Camera *camera, float speed);
void camera_move_up(Camera *camera, float speed);

Mat4 camera_view_matrix(const Camera *camera);

struct _Camera_Controller
{
    IC_KEY forward_key;
    IC_KEY backward_key;
    IC_KEY left_key;
    IC_KEY right_key;

    IC_KEY up_key;
    IC_KEY down_key;

    float xz_speed;
    float y_speed;

    float mouse_sensitivity;

    Camera *camera;
};

typedef struct _Camera_Controller Camera_Controller;

typedef struct _Timer Timer;

void camera_controller_update(const Camera_Controller *controller, const Input *input, const Timer *timer);