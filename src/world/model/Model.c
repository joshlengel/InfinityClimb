#include"world/model/Model.h"

#include"world/model/mesh/Mesh.h"

const Vec3 ZERO = { 0.0f, 0.0f, 0.0f };

Mat4 model_transform_matrix(const Model *model)
{
    Mat4 scale = mat4_make_scale(model->scale.x, model->scale.y, model->scale.z);
    Mat4 rotate = mat4_make_axis_angle(&model->rotation_axis, model->rotation);
    Mat4 translate = mat4_make_translate(model->position.x, model->position.y, model->position.z);

    Mat4 res = mat4_mul(&rotate, &scale);
    res = mat4_mul(&translate, &res);
    return res;
}

void model_render(const Model *model)
{
    mesh_render(model->mesh);
}

void model_update(Model *model, float dt)
{
    Vec3 sum_acceleration = vec3_add(&model->acceleration, &model->temp_acceleration);
    Vec3 d_vel = vec3_scale(&sum_acceleration, dt);
    model->velocity = vec3_add(&model->velocity, &d_vel);

    Vec3 sum_velocity = vec3_add(&model->velocity, &model->temp_velocity);
    Vec3 d_pos = vec3_scale(&sum_velocity, dt);
    model->position = vec3_add(&model->position, &d_pos);

    model->temp_acceleration = ZERO;
    model->temp_velocity = ZERO;
}