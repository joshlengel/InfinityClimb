#pragma once

#include"Core.h"
#include"world/model/Shader.h"

struct _Gui_Shader
{
    Shader shader;
};

typedef struct _Gui_Shader Gui_Shader;

typedef struct _Vec2 Vec2;
typedef struct _Color Color;

IC_ERROR_CODE gui_shader_create(Gui_Shader *dest);
void gui_shader_destroy(const Gui_Shader *shader);

void gui_shader_bind(const Gui_Shader *shader);
void gui_shader_set_location(const Gui_Shader *shader, const Vec2 *center, const Vec2 *extents);
void gui_shader_set_color(const Gui_Shader *shader, const Color *color);