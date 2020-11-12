#include"gui/Gui_Shader.h"

#include"util/math/Vec.h"
#include"util/Color.h"
#include"util/Utils.h"

static const char *const GUI_V_PATH = "assets/shaders/gui.vert";
static const char *const GUI_F_PATH = "assets/shaders/gui.frag";

IC_ERROR_CODE gui_shader_create(Gui_Shader *dest)
{
    IC_ERROR_CODE ec;

    String v_src = read_source(GUI_V_PATH, &ec);
    if (ec != IC_NO_ERROR) return ec;

    String f_src = read_source(GUI_F_PATH, &ec);
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

    shader_declare_uniform(&dest->shader, "location");
    shader_declare_uniform(&dest->shader, "base_color");

    return IC_NO_ERROR;
}

void gui_shader_destroy(const Gui_Shader *shader)
{
    shader_destroy(&shader->shader);
}

void gui_shader_bind(const Gui_Shader *shader)
{
    shader_bind(&shader->shader);
}

void gui_shader_set_location(const Gui_Shader *shader, const Vec2 *center, const Vec2 *extents)
{
    shader_set_uniform_4f(&shader->shader, "location", center->x, center->y, extents->x, extents->y);
}

void gui_shader_set_color(const Gui_Shader *shader, const Color *color)
{
    shader_set_uniform_4f(&shader->shader, "base_color", color->red_f, color->green_f, color->blue_f, color->alpha_f);
}