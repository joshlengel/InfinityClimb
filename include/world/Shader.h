#pragma once

#include"Core.h"

#include<stdint.h>

typedef struct _Shader_Data Shader_Data;

struct _Shader
{
    const char *vertex_source;
    const char *fragment_source;

    uint32_t num_uniforms;

    Shader_Data *data;
};

typedef struct _Shader Shader;

IC_ERROR_CODE shader_create(Shader *dest);
void shader_destroy(const Shader *shader);

void shader_declare_uniform(const Shader *shader, const char *name);
void shader_set_uniform_1i(const Shader *shader, const char *name, int32_t v);
void shader_set_uniform_2i(const Shader *shader, const char *name, int32_t v1, int32_t v2);
void shader_set_uniform_3i(const Shader *shader, const char *name, int32_t v1, int32_t v2, int32_t v3);
void shader_set_uniform_4i(const Shader *shader, const char *name, int32_t v1, int32_t v2, int32_t v3, int32_t v4);
void shader_set_uniform_1f(const Shader *shader, const char *name, float v);
void shader_set_uniform_2f(const Shader *shader, const char *name, float v1, float v2);
void shader_set_uniform_3f(const Shader *shader, const char *name, float v1, float v2, float v3);
void shader_set_uniform_4f(const Shader *shader, const char *name, float v1, float v2, float v3, float v4);
void shader_set_uniform_mat3(const Shader *shader, const char *name, float *values);
void shader_set_uniform_mat4(const Shader *shader, const char *name, float *values);

void shader_bind(const Shader *shader);