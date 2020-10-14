#include"window/Window.h"
#include"window/Context.h"
#include"world/Rect.h"
#include"world/Shader.h"
#include"Libs.h"
#include"Log.h"
#include"Utils.h"

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
        log_trace("Error loading libs. Error code: %i", ec);
        return -1;
    }

    window.width = 800;
    window.height = 600;
    window.title = "Hello World!";

    if ((ec = window_create(&window)) != IC_NO_ERROR)
    {
        log_trace("Error creating window. Error code: %i", ec);
        return -1;
    }

    Vec3 rect_pos = { 0.0f, 0.0f, 0.0f };
    Vec3 rect_scale = { 1.0f, 1.0f, 1.0f };

    rect.position = rect_pos;
    rect.scale = rect_scale;

    rect_create(&rect);

    shader.vertex_source = read_source("../assets/shaders/rect.vert", &ec);
    if (ec != IC_NO_ERROR)
    {
        if (ec == IC_READ_CLOSE_FILE_ERROR) free((void*)shader.vertex_source);
        
        rect_destroy(&rect);
        window_destroy(&window);
        return -1;
    }

    shader.fragment_source = read_source("../assets/shaders/rect.frag", &ec);
    if (ec != IC_NO_ERROR)
    {
        if (ec == IC_READ_CLOSE_FILE_ERROR) free((void*)shader.fragment_source);
        
        rect_destroy(&rect);
        window_destroy(&window);
        return -1;
    }

    shader.num_uniforms = 2;

    if ((ec = shader_create(&shader)) != IC_NO_ERROR)
    {
        log_trace("Error creating shader. Error code: %i", ec);

        free((void*)shader.vertex_source);
        free((void*)shader.fragment_source);
        rect_destroy(&rect);
        window_destroy(&window);
        return -1;
    }

    free((void*)shader.vertex_source);
    free((void*)shader.fragment_source);

    
    shader_declare_uniform(&shader, "base_color");

    context_background_color(1.0f, 0.0f, 0.0f, 1.0f);

    return 0;
}

int main(int argc, char **argv)
{
    if (init() != 0)
    {
        FILE *init_log = fopen("../logs/init_log.txt", "w");
        dump_log(init_log);
        fclose(init_log);

        terminate_libs();
        return -1;
    }
    else
    {
        remove("../logs/init_log.txt");
    }
    

    window_show(&window);

    while(!window_should_close(&window))
    {
        window_poll_events();

        context_clear();

        shader_bind(&shader);
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