#include"world/rect/Rect_Shader.h"
#include"world/rect/Rect.h"

#include"util/Utils.h"

#include<stdlib.h>

IC_ERROR_CODE rect_shader_create(Rect_Shader *dest)
{
    IC_ERROR_CODE ec;

    String v_src = read_source("../assets/shaders/rect.vert", &ec);
    if (ec != IC_NO_ERROR) return ec;

    String f_src = read_source("../assets/shaders/rect.frag", &ec);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        return ec;
    }

    string_view_create_s(&dest->shader.vertex_source, &v_src, 0, UINT32_MAX);
    string_view_create_s(&dest->shader.fragment_source, &f_src, 0, UINT32_MAX);

    dest->shader.num_uniforms = 6;

    ec = shader_create(&dest->shader);

    if (ec != IC_NO_ERROR) return ec;

    // Delete allocated strings for shader source code
    string_destroy(&v_src);
    string_destroy(&f_src);

    // Declare shader uniforms
    shader_declare_uniform(&dest->shader, "transform");
    shader_declare_uniform(&dest->shader, "view");
    shader_declare_uniform(&dest->shader, "projection");
    shader_declare_uniform(&dest->shader, "base_color");
    shader_declare_uniform(&dest->shader, "light_position");
    shader_declare_uniform(&dest->shader, "camera_position");

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

void rect_shader_set_light(const Rect_Shader *shader, const Vec3 *light_pos)
{
    shader_set_uniform_3f(&shader->shader, "light_position", light_pos->x, light_pos->y, light_pos->z);
}

void rect_shader_set_camera_pos(const Rect_Shader *shader, const Vec3 *camera_pos)
{
    shader_set_uniform_3f(&shader->shader, "camera_position", camera_pos->x, camera_pos->y, camera_pos->z);
}