#include"world/rect/Rect_Shader.h"
#include"world/rect/Rect.h"

#include"Utils.h"

#include<stdlib.h>

IC_ERROR_CODE rect_shader_create(Rect_Shader *dest)
{
    IC_ERROR_CODE ec;

    dest->shader.vertex_source = read_source("../assets/shaders/rect.vert", &ec);
    if (ec != IC_NO_ERROR) return ec;

    dest->shader.fragment_source = read_source("../assets/shaders/rect.frag", &ec);
    if (ec != IC_NO_ERROR) return ec;

    dest->shader.num_uniforms = 5;

    ec = shader_create(&dest->shader);

    if (ec != IC_NO_ERROR) return ec;

    // Delete allocated strings for shader source code
    free((void*)dest->shader.vertex_source);
    free((void*)dest->shader.fragment_source);

    // Declare shader uniforms
    shader_declare_uniform(&dest->shader, "transform");
    shader_declare_uniform(&dest->shader, "view");
    shader_declare_uniform(&dest->shader, "projection");
    shader_declare_uniform(&dest->shader, "base_color");
    shader_declare_uniform(&dest->shader, "light_dir");

    return IC_NO_ERROR;
}

void rect_shader_destroy(const Rect_Shader *shader)
{
    shader_destroy(&shader->shader);
}

void rect_shader_bind(const Rect_Shader *shader)
{
    shader_bind(&shader->shader);
}

void rect_shader_set_view_projection(const Rect_Shader *shader, const Mat4 *view, const Mat4 *projection)
{
    float mat_buffer[16];
    
    mat4_load(view, mat_buffer);
    shader_set_uniform_mat4(&shader->shader, "view", mat_buffer);

    mat4_load(projection, mat_buffer);
    shader_set_uniform_mat4(&shader->shader, "projection", mat_buffer);
}

void rect_shader_set_rect(const Rect_Shader *shader, const Rect *rect)
{
    float mat_buffer[16];

    Mat4 transform = rect_transform_matrix(rect);
    mat4_load(&transform, mat_buffer);
    shader_set_uniform_mat4(&shader->shader, "transform", mat_buffer);
}

void rect_shader_set_color(const Rect_Shader *shader, const Color *color)
{
    shader_set_uniform_4f(&shader->shader, "base_color", color->red_f, color->green_f, color->blue_f, color->alpha_f);
}

void rect_shader_set_light(const Rect_Shader *shader, const Vec3 *direction)
{
    shader_set_uniform_3f(&shader->shader, "light_dir", direction->x, direction->y, direction->z);
}