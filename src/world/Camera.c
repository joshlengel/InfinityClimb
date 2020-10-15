#include"world/Camera.h"
#include"window/Input.h"
#include"Utils.h"

#include<math.h>

void camera_move_forward(Camera *camera, float speed)
{
    camera->position.x += sinf(camera->yaw) * speed;
    camera->position.z += cosf(camera->yaw) * speed;
}

void camera_move_right(Camera *camera, float speed)
{
    camera->position.x += cosf(camera->yaw) * speed;
    camera->position.z -= sinf(camera->yaw) * speed;
}

void camera_move_up(Camera *camera, float speed)
{
    camera->position.y += speed;
}

Mat4 camera_view_matrix(const Camera *camera)
{
    Mat4 translate = mat4_make_translate(-camera->position.x, -camera->position.y, -camera->position.z);
    
    float sin_p = sinf(camera->pitch);
    float cos_p = cosf(camera->pitch);
    float sin_y = sinf(camera->yaw);
    float cos_y = cosf(camera->yaw);

    Mat4 rot_pitch =
    {
        1.0f,   0.0f,  0.0f, 0.0f,
        0.0f,  cos_p, sin_p, 0.0f,
        0.0f, -sin_p, cos_p, 0.0f,
        0.0f,   0.0f,  0.0f, 1.0f
    };

    Mat4 rot_yaw =
    {
         cos_y, 0.0f, sin_y, 0.0f,
          0.0f, 1.0f,  0.0f, 0.0f,
        -sin_y, 0.0f, cos_y, 0.0f,
          0.0f, 0.0f,  0.0f, 1.0f
    };

    Mat4 rot = mat4_mul(&rot_yaw, &rot_pitch);
    
    Mat4 res = mat4_mul(&rot, &translate);
    return res;
}

void camera_controller_update(const Camera_Controller *controller, const Input *input, const Timer *timer)
{
    float s_xz_speed = scale_speed(timer, controller->xz_speed);
    float s_y_speed = scale_speed(timer, controller->y_speed);

    if (input_key_down(input, controller->forward_key))
    {
        camera_move_forward(controller->camera, s_xz_speed);
    }

    if (input_key_down(input, controller->backward_key))
    {
        camera_move_forward(controller->camera, -s_xz_speed);
    }

    if (input_key_down(input, controller->right_key))
    {
        camera_move_right(controller->camera, s_xz_speed);
    }

    if (input_key_down(input, controller->left_key))
    {
        camera_move_right(controller->camera, -s_xz_speed);
    }

    if (input_key_down(input, controller->up_key))
    {
        camera_move_up(controller->camera, s_y_speed);
    }

    if (input_key_down(input, controller->down_key))
    {
        camera_move_up(controller->camera, -s_y_speed);
    }

    controller->camera->yaw += input_cursor_dx(input) * controller->mouse_sensitivity;
    controller->camera->pitch += input_cursor_dy(input) * controller->mouse_sensitivity;
}