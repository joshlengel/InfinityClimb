#include"world/model/mesh/Mesh_Shader.h"

#include"util/Utils.h"
#include"util/Color.h"
#include"util/math/Mat.h"

const char *MESH_V_PATH = "assets/shaders/mesh.vert";
const char *MESH_F_PATH = "assets/shaders/mesh.frag";

IC_ERROR_CODE mesh_shader_create(Mesh_Shader *dest)
{
    IC_ERROR_CODE ec;

    String v_src = read_source(MESH_V_PATH, &ec);
    if (ec != IC_NO_ERROR) return ec;

    String f_src = read_source(MESH_F_PATH, &ec);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        return ec;
    }

    string_view_create_s(&dest->shader.vertex_source, &v_src, 0, UINT32_MAX);
    string_view_create_s(&dest->shader.fragment_source, &f_src, 0, UINT32_MAX);

    dest->shader.num_uniforms = 4;

    ec = shader_create(&dest->shader);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        string_destroy(&f_src);
        return ec;
    }

    string_destroy(&v_src);
    string_destroy(&f_src);

    shader_declare_uniform(&dest->shader, "transform");
    shader_declare_uniform(&dest->shader, "view");
    shader_declare_uniform(&dest->shader, "projection");
    shader_declare_uniform(&dest->shader, "base_color");

    return IC_NO_ERROR;
}

void mesh_shader_destroy(const Mesh_Shader *shader)
{
    shader_destroy(&shader->shader);
}

void mesh_shader_bind(const Mesh_Shader *shader)
{
    shader_bind(&shader->shader);
}

void mesh_shader_set_transform(const Mesh_Shader *shader, const Mat4 *transform)
{
    float buffer[16];
    mat4_load(transform, buffer);
    shader_set_uniform_mat4(&shader->shader, "transform", buffer);
}

void mesh_shader_set_view(const Mesh_Shader *shader, const Mat4 *view)
{
    float buffer[16];
    mat4_load(view, buffer);
    shader_set_uniform_mat4(&shader->shader, "view", buffer);
}

void mesh_shader_set_projection(const Mesh_Shader *shader, const Mat4 *projection)
{
    float buffer[16];
    mat4_load(projection, buffer);
    shader_set_uniform_mat4(&shader->shader, "projection", buffer);
}

void mesh_shader_set_color(const Mesh_Shader *shader, const Color *color)
{
    shader_set_uniform_4f(&shader->shader, "base_color", color->red_f, color->green_f, color->blue_f, color->alpha_f);
}