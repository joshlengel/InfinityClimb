#include"gui/components/Component.h"

#include"gui/Gui_Shader.h"

#include"window/Window.h"
#include"Log.h"

#include<stdlib.h>
#include<glad/glad.h>

extern GLuint gui_vao_id;

static const Vec2 PANE_CENTER = { 0.0f, 0.0f };
static const Vec2 PANE_EXTENTS = { 1.0f, 1.0f };

void gui_pane_render(const Gui_Pane *pane, const Gui_Shader *shader, const Window *window)
{
    gui_shader_bind(shader);

    if (pane->fixed)
    {
        float aspect_ratio = window_aspect_ratio(window);
        Vec2 center = pane->center;
        Vec2 extents = pane->extents;

        if (aspect_ratio < 1.0f)
        {
            center.y *= aspect_ratio;
            extents.y *= aspect_ratio;
        }
        else
        {
            center.x /= aspect_ratio;
            extents.x /= aspect_ratio;
        }

        gui_shader_set_location(shader, &center, &extents);
    }
    else
    {
        gui_shader_set_location(shader, &pane->center, &pane->extents);
    }

    gui_shader_set_color(shader, &pane->color);
    
    glBindVertexArray(gui_vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void gui_panel_render(const Gui_Panel *panel, const Gui_Shader *shader)
{
    gui_shader_bind(shader);
    gui_shader_set_location(shader, &PANE_CENTER, &PANE_EXTENTS);
    gui_shader_set_color(shader, &panel->color);

    glBindVertexArray(gui_vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}