#include"math/Mat.h"
#include"math/Color.h"
#include"window/Window.h"
#include"window/Input.h"
#include"window/Context.h"
#include"world/Rect.h"
#include"world/Shader.h"
#include"world/Camera.h"
#include"Libs.h"
#include"Log.h"
#include"Utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

Window window;
Input input;
Rect rect;
Shader shader;
Camera camera;

Vec3 LIGHT_DIR = { -0.2f, -0.8f, 0.4f };

Color SKY_COLOR;

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

    context_init();

    input.window = &window;
    input_create(&input);

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

    shader.num_uniforms = 5;

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
    
    shader_declare_uniform(&shader, "transform");
    shader_declare_uniform(&shader, "view");
    shader_declare_uniform(&shader, "projection");
    shader_declare_uniform(&shader, "base_color");
    shader_declare_uniform(&shader, "light_dir");

    LIGHT_DIR = vec3_normalize(&LIGHT_DIR);

    Vec3 cam_pos = { 0.0f, 0.0f, -2.0f };
    camera.position = cam_pos;

    SKY_COLOR = color_create_hex(0x87CEEBFF);
    context_background_color(&SKY_COLOR);

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
        input_update(&input);
        window_poll_events();

        context_clear();

        if (input_key_down(&input, IC_KEY_UP))
        {
            camera_move_forward(&camera, 0.01f);
        }
        else if (input_key_down(&input, IC_KEY_DOWN))
        {
            camera_move_forward(&camera, -0.01f);
        }
        else if (input_key_down(&input, IC_KEY_LEFT))
        {
            camera.yaw -= 0.1f;
        }
        else if (input_key_down(&input, IC_KEY_RIGHT))
        {
            camera.yaw += 0.1f;
        }

        Mat4 translate = mat4_make_translate(0.25f, -0.5f, 0.0f);
        Mat4 scale = mat4_make_scale(0.5f, 0.4f, 1.0f);

        shader_bind(&shader);
        
        float mat_buffer[16];
        
        Mat4 transform = mat4_mul(&translate, &scale);
        mat4_load(&transform, mat_buffer);
        shader_set_uniform_mat4(&shader, "transform", mat_buffer);

        Mat4 view = camera_view_matrix(&camera);
        mat4_load(&view, mat_buffer);
        shader_set_uniform_mat4(&shader, "view", mat_buffer);

        Mat4 projection = mat4_make_project(M_PI_2, window_aspect_ratio(&window), 0.1f, 100.0f);
        mat4_load(&projection, mat_buffer);
        shader_set_uniform_mat4(&shader, "projection", mat_buffer);

        shader_set_uniform_4f(&shader, "base_color", 0.4f, 0.4f, 0.4f, 1.0f);
        shader_set_uniform_3f(&shader, "light_dir", LIGHT_DIR.x, LIGHT_DIR.y, LIGHT_DIR.z);
        rect_render(&rect);

        window_swap_buffers(&window);
    }

    rect_destroy(&rect);
    shader_destroy(&shader);
    window_destroy(&window);

    terminate_libs();

    return 0;
}