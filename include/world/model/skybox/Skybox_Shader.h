#pragma once

#include"world/model/Shader.h"
#include"Core.h"

struct _Skybox_Shader
{
    Shader shader;
};

typedef struct _Skybox_Shader Skybox_Shader;

typedef struct _Skybox Skybox;
typedef struct _Mat4 Mat4;

IC_ERROR_CODE skybox_shader_create(Skybox_Shader *dest);
void skybox_shader_destroy(const Skybox_Shader *shader);

void skybox_shader_bind(const Skybox_Shader *shader);
void skybox_shader_set_view_projection(const Skybox_Shader *shader, const Mat4 *view, const Mat4 *projection);
void skybox_shader_set_skybox(const Skybox_Shader *shader, const Skybox *skybox);