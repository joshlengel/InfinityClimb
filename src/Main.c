#include"window/Window.h"
#include"window/Context.h"
#include"world/Rect.h"
#include"world/Shader.h"
#include"Libs.h"
#include"Log.h"

#include<stdio.h>
#include<stdlib.h>

Window window;
Rect rect;
Shader shader;

int init()
{
    IC_ERROR_CODE ec;

    if ((ec = load_libs()) != IC_NO_ERROR)
    {
        log_trace("Error loading libs. Error code: %i\n", ec);
        return -1;
    }

    window.width = 800;
    window.height = 600;
    window.title = "Hello World!";

    if ((ec = window_create(&window)) != IC_NO_ERROR)
    {
        log_trace("Error creating window. Error code: %i\n", ec);
        terminate_libs();
        return -1;
    }

    Vec3 rect_pos = { 0.0f, 0.0f, 0.0f };
    Vec3 rect_scale = { 1.0f, 1.0f, 1.0f };

    rect.position = rect_pos;
    rect.scale = rect_scale;

    rect_create(&rect);

    shader.vertex_source =
        "#version 330\n"
        "layout(location=0) in vec3 vertex;\n"
        "uniform float scale;\n"
        "void main() {\n"
        "   gl_Position = vec4(vertex * scale, 1.0);\n"
        "}";

    shader.fragment_source =
        "#version 330\n"
        "out vec4 color;\n"
        "uniform vec4 base_color;\n"
        "void main() {\n"
        "   color = base_color;\n"
        "}";

    shader.num_uniforms = 2;

    if ((ec = shader_create(&shader)) != IC_NO_ERROR)
    {
        log_trace("Error creating shader. Error code: %i\n", ec);

        rect_destroy(&rect);
        window_destroy(&window);
        terminate_libs();
        return -1;
    }

    shader_declare_uniform(&shader, "scale");
    shader_declare_uniform(&shader, "base_color");

    context_background_color(1.0f, 0.0f, 0.0f, 1.0f);

    return 0;
}

int main(int argc, char **argv)
{
    if (init() != 0)
    {
        FILE *init_log = fopen("/home/josh/Dev/c++/InfinityClimb/logs/init_log.txt", "w");
        dump_log(init_log);
        fclose(init_log);
        return -1;
    }

    window_show(&window);

    while(!window_should_close(&window))
    {
        window_poll_events();

        context_clear();

        shader_bind(&shader);
        shader_set_uniform_1f(&shader, "scale", 0.5f);
        shader_set_uniform_4f(&shader, "base_color", 0.5f, 0.7f, 0.9f, 1.0f);
        rect_render(&rect);

        window_swap_buffers(&window);
    }

    rect_destroy(&rect);
    shader_destroy(&shader);
    window_destroy(&window);

    terminate_libs();

    return 0;
}