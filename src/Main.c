#include"math/Mat.h"
#include"math/Color.h"
#include"window/Window.h"
#include"window/Input.h"
#include"window/Context.h"
#include"world/rect/Rect.h"
#include"world/player/Player.h"
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
Player player;
Player_Controller player_controller;

Vec3 LIGHT_DIR = { -0.2f, -0.8f, 0.4f };
Color SKY_COLOR;

const uint16_t FOV = 70;

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

    player.position.x = 0.0f;
    player.position.y = 2.0f;
    player.position.z = 0.0f;
    
    player_controller.player = &player;
    player_controller.xz_speed = 0.7f;
    player_controller.y_speed = 0.9f;
    player_controller.mouse_sensitivity = 0.001f;

    player_controller.forward_key  = IC_KEY_W;
    player_controller.backward_key = IC_KEY_S;
    player_controller.right_key    = IC_KEY_D;
    player_controller.left_key     = IC_KEY_A;
    player_controller.up_key       = IC_KEY_SPACE;
    player_controller.down_key     = IC_KEY_SHIFT;

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

    input_disable_cursor(&input);
    
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

        // Check to see if user pressed escape to bring cursor back
        if (input_key_pressed(&input, IC_KEY_ESCAPE))
        {
            input_toggle_cursor(&input);
        }

        if (!input_cursor_enabled(&input))
        {
            // Player movement
            player_controller_update(&player_controller, &input, timer_get_dt(&timer));

            // First person camera
            camera.position = player.position;
            camera.pitch = player.pitch;
            camera.yaw = player.yaw;
        }

        // TODO: Create 'renderable' to automaticially retrieve data for shader uniforms
        shader_bind(&shader);
        
        float mat_buffer[16];
        
        Mat4 transform = rect_transform_matrix(&rect);
        mat4_load(&transform, mat_buffer);
        shader_set_uniform_mat4(&shader, "transform", mat_buffer);

        Mat4 view = camera_view_matrix(&camera);
        mat4_load(&view, mat_buffer);
        shader_set_uniform_mat4(&shader, "view", mat_buffer);

        Mat4 projection = mat4_make_project((float)FOV / 180 * M_PI, window_aspect_ratio(&window), 0.1f, 100.0f);
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