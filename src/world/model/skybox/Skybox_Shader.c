#include"world/model/skybox/Skybox_Shader.h"
#include"world/model/skybox/Skybox.h"
#include"util/math/Mat.h"
#include"util/Utils.h"

#include<stdlib.h>

IC_ERROR_CODE skybox_shader_create(Skybox_Shader *dest)
{
    IC_ERROR_CODE ec;

    String v_src = read_source("../../assets/shaders/skybox.vert", &ec);
    if (ec != IC_NO_ERROR) return ec;

    String f_src = read_source("../../assets/shaders/skybox.frag", &ec);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        return ec;
    }

    string_view_create_s(&dest->shader.vertex_source, &v_src, 0, UINT32_MAX);
    string_view_create_s(&dest->shader.fragment_source, &f_src, 0, UINT32_MAX);

    dest->shader.num_uniforms = 3;

    ec = shader_create(&dest->shader);

    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        string_destroy(&f_src);
        return ec;
    }

    // Delete allocated strings for shader source code
    string_destroy(&v_src);
    string_destroy(&f_src);

    // Declare shader uniforms
    shader_declare_uniform(&dest->shader, "view");
    shader_declare_uniform(&dest->shader, "projection");
    shader_declare_uniform(&dest->shader, "scale");

    return IC_NO_ERROR;
}

void skybox_shader_destroy(const Skybox_Shader *shader)
{
    shader_destroy(&shader->shader);
}

void skybox_shader_bind(const Skybox_Shader *shader)
{
    shader_bind(&shader->shader);
}

void skybox_shader_set_view_projection(const Skybox_Shader *shader, const Mat4 *view, const Mat4 *projection)
{
    float mat_buffer[16];
    
    mat4_load(view, mat_buffer);
    shader_set_uniform_mat4(&shader->shader, "view", mat_buffer);

    mat4_load(projection, mat_buffer);
    shader_set_uniform_mat4(&shader->shader, "projection", mat_buffer);
}

void skybox_shader_set_skybox(const Skybox_Shader *shader, const Skybox *skybox)
{
    shader_set_uniform_1f(&shader->shader, "scale", skybox->size);
}