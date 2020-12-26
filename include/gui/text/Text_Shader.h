#pragma once

#include"world/model/Shader.h"

struct _Text_Shader
{
    Shader shader;
};

typedef struct _Text_Shader Text_Shader;

typedef struct _Color Color;

IC_ERROR_CODE text_shader_create(Text_Shader *dest);
void text_shader_destroy(const Text_Shader *shader);

void text_shader_bind(const Text_Shader *shader);
void text_shader_set_aspect_ratio(const Text_Shader *shader, float aspect_ratio);
void text_shader_set_color(const Text_Shader *shader, const Color *color);