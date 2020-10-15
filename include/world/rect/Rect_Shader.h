#pragma once

#include"world/Shader.h"
#include"math/Vec.h"
#include"math/Mat.h"
#include"math/Color.h"

struct _Rect_Shader
{
    Shader shader;
};

typedef struct _Rect_Shader Rect_Shader;

typedef struct _Rect Rect;

IC_ERROR_CODE rect_shader_create(Rect_Shader *shader);
void rect_shader_destroy(const Rect_Shader *shader);

void rect_shader_bind(const Rect_Shader *shader);
void rect_shader_set_view_projection(const Rect_Shader *shader, const Mat4 *view, const Mat4 *projection);
void rect_shader_set_rect(const Rect_Shader *shader, const Rect *rect);
void rect_shader_set_color(const Rect_Shader *shader, const Color *color);
void rect_shader_set_light(const Rect_Shader *shader, const Vec3 *direction);