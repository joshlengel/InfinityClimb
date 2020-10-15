#include"world/Camera.h"

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