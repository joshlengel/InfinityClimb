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

Loader loader;
Timer timer;
    
Window window;
Context context;
Input input;

Rect rect;
Shader shader;
Camera camera;

Vec3 LIGHT_DIR = { -0.2f, -0.8f, 0.4f };
Color SKY_COLOR;

const float CAM_MOVE_SPEED = 0.7f;
const float CAM_ROTATE_SPEED = 2.0f;

int init()
{
    IC_ERROR_CODE ec;

    // Load libraries
    if ((ec = load_libs()) != IC_NO_ERROR)
    {
        log_trace("Error loading libs. Error code: %i", ec);
        return -1;
    }

    // Timer
    timer.fps = 120;

    // Window
    window.width = 800;
    window.height = 600;
    window.title = "Hello World!";

    input.window = &window;

    // Context
    SKY_COLOR = color_create_hex(0x87CEEBFF);
    
    context.background_color = &SKY_COLOR;
    context.clear_color = IC_TRUE;
    context.clear_depth = IC_TRUE;

    // Shader
    shader.vertex_source = read_source("../assets/shaders/rect.vert", &ec);
    if (ec != IC_NO_ERROR) return -1;

    shader.fragment_source = read_source("../assets/shaders/rect.frag", &ec);
    if (ec != IC_NO_ERROR) return -1;

    shader.num_uniforms = 5;

    // Rectangle
    Vec3 rect_pos = { 0.0f, 0.0f, 0.0f };
    Vec3 rect_scale = { 1.0f, 1.0f, 1.0f };

    rect.position = rect_pos;
    rect.scale = rect_scale;

    // Loader
    loader.num_resources = 6;

    loader_create(&loader);
    loader_add_resource(&loader, &timer,   (Loader_Init_proc)timer_create,   (Loader_Dest_proc)timer_destroy);
    loader_add_resource(&loader, &window,  (Loader_Init_proc)window_create,  (Loader_Dest_proc)window_destroy);
    loader_add_resource(&loader, &input,   (Loader_Init_proc)input_create,   (Loader_Dest_proc)input_destroy);
    loader_add_resource(&loader, &context, (Loader_Init_proc)context_create, (Loader_Dest_proc)context_destroy);
    loader_add_resource(&loader, &rect,    (Loader_Init_proc)rect_create,    (Loader_Dest_proc)rect_destroy);
    loader_add_resource(&loader, &shader,  (Loader_Init_proc)shader_create,  (Loader_Dest_proc)shader_destroy);
    loader_load(&loader);

    if (loader_error(&loader) != IC_NO_ERROR) return -1;
    
    // Delete allocated strings for shader source code
    free((void*)shader.vertex_source);
    free((void*)shader.fragment_source);

    // Update context
    context_update(&context);

    // Declare shader uniforms
    shader_declare_uniform(&shader, "transform");
    shader_declare_uniform(&shader, "view");
    shader_declare_uniform(&shader, "projection");
    shader_declare_uniform(&shader, "base_color");
    shader_declare_uniform(&shader, "light_dir");

    // Initialize world aspects (Light, Camera, etc.)
    LIGHT_DIR = vec3_normalize(&LIGHT_DIR);

    Vec3 cam_pos = { 0.0f, 0.0f, -2.0f };
    camera.position = cam_pos;

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
    timer_start(&timer);

    while(!window_should_close(&window))
    {
        if (!timer_should_update(&timer)) continue;

        // Update input
        input_update(&input);
        window_poll_events();

        // Clear screen
        context_clear(&context);

        // Calculate scaled camera speed
        float cam_move_speed_s = scale_speed(&timer, CAM_MOVE_SPEED);
        float cam_rotate_speed_s = scale_speed(&timer, CAM_ROTATE_SPEED);

        // TODO: Create camera controller
        if (input_key_down(&input, IC_KEY_UP))
        {
            camera_move_forward(&camera, cam_move_speed_s);
        }
        else if (input_key_down(&input, IC_KEY_DOWN))
        {
            camera_move_forward(&camera, -cam_move_speed_s);
        }
        else if (input_key_down(&input, IC_KEY_LEFT))
        {
            camera.yaw -= cam_rotate_speed_s;
        }
        else if (input_key_down(&input, IC_KEY_RIGHT))
        {
            camera.yaw += cam_rotate_speed_s;
        }

        // TODO: Create 'renderable' to automaticially retrieve data for shader uniforms
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

    loader_unload(&loader);
    loader_destroy(&loader);

    terminate_libs();

    return 0;
}