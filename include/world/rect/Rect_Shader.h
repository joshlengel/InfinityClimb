#pragma once

#include"world/Shader.h"
#include"util/math/Vec.h"
#include"util/math/Mat.h"
#include"util/Color.h"

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
void rect_shader_set_light(const Rect_Shader *shader, const Vec3 *position);
void rect_shader_set_camera_pos(const Rect_Shader *shader, const Vec3 *position);