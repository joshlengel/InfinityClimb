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