#include"gui/text/Text_Shader.h"

#include"util/Utils.h"
#include"util/Color.h"

static const char *const TEXT_V_PATH = "assets/shaders/text.vert";
static const char *const TEXT_F_PATH = "assets/shaders/text.frag";

IC_ERROR_CODE text_shader_create(Text_Shader *dest)
{
    IC_ERROR_CODE ec;

    String v_src = read_source(TEXT_V_PATH, &ec);
    if (ec != IC_NO_ERROR) return ec;

    String f_src = read_source(TEXT_F_PATH, &ec);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        return ec;
    }

    string_view_create_s(&dest->shader.vertex_source, &v_src, 0, UINT32_MAX);
    string_view_create_s(&dest->shader.fragment_source, &f_src, 0, UINT32_MAX);

    dest->shader.num_uniforms = 2;

    ec = shader_create(&dest->shader);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        string_destroy(&f_src);
        return ec;
    }

    string_destroy(&v_src);
    string_destroy(&f_src);

    shader_declare_uniform(&dest->shader, "aspect_ratio");
    shader_declare_uniform(&dest->shader, "base_color");
    
    return IC_NO_ERROR;
}

void text_shader_destroy(const Text_Shader *shader)
{
    shader_destroy(&shader->shader);
}

void text_shader_bind(const Text_Shader *shader)
{
    shader_bind(&shader->shader);
}

void text_shader_set_aspect_ratio(const Text_Shader *shader, float aspect_ratio)
{
    shader_set_uniform_1f(&shader->shader, "aspect_ratio", aspect_ratio);
}

void text_shader_set_color(const Text_Shader *shader, const Color *color)
{
    shader_set_uniform_4f(&shader->shader, "base_color", color->red_f, color->green_f, color->blue_f, color->alpha_f);
}