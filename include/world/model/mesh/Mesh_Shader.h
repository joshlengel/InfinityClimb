#pragma once

#include"Core.h"
#include"world/model/Shader.h"

struct _Mesh_Shader
{
    Shader shader;
};

typedef struct _Mesh_Shader Mesh_Shader;

typedef struct _Mat4 Mat4;

IC_ERROR_CODE mesh_shader_create(Mesh_Shader *dest);
void mesh_shader_destroy(const Mesh_Shader *shader);

void mesh_shader_bind(const Mesh_Shader *shader);
void mesh_shader_set_transform(const Mesh_Shader *shader, const Mat4 *transform);
void mesh_shader_set_view(const Mesh_Shader *shader, const Mat4 *view);
void mesh_shader_set_projection(const Mesh_Shader *shader, const Mat4 *projection);