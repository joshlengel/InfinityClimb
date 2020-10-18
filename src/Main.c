#include"util/Utils.h"
#include"util/Color.h"
#include"util/math/Mat.h"
#include"window/Window.h"
#include"window/Input.h"
#include"window/Context.h"
#include"world/Camera.h"
#include"world/model/skybox/Skybox.h"
#include"world/model/skybox/Skybox_Shader.h"
#include"world/model/mesh/Mesh.h"
#include"world/model/mesh/Mesh_Shader.h"
#include"world/player/Player.h"
#include"Libs.h"
#include"Log.h"

#include<stdio.h>
#include<stdlib.h>

Loader loader;
Timer timer;
    
Window window;
Context context;
Input input;

Camera camera;
Player player;
Player_Controller player_controller;

Mesh mesh;
Mesh_Shader mesh_shader;

Skybox skybox;
Skybox_Shader skybox_shader;

Vec3 LIGHT_POSITION = { -30.0f, 40.0f, 20.0f };
Color SKY_COLOR;

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
    context.cull = IC_TRUE;
    context.cull_front = IC_FALSE;

    // Mesh
    float mesh_vertices[] =
    {
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f
    };

    float mesh_normals[] =
    {
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,

         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,

         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,

         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,

         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f
    };

    uint32_t mesh_indices[] =
    {
         0,  1,  3,  0,  3,  2,
         4,  5,  7,  4,  7,  6,
         8,  9, 11,  8, 11, 10,
        12, 13, 15, 12, 15, 14,
        16, 17, 19, 16, 19, 18,
        20, 21, 23, 20, 23, 22
    };

    mesh.vertices       = mesh_vertices;
    mesh.texture_coords = NULL;
    mesh.normals        = mesh_normals;
    mesh.num_vertices   = 24;

    mesh.indices     = mesh_indices;
    mesh.num_indices = 36;

    // Skybox
    skybox.left_tex_path   = "../assets/textures/left.png";
    skybox.right_tex_path  = "../assets/textures/right.png";
    skybox.bottom_tex_path = "../assets/textures/bottom.png";
    skybox.top_tex_path    = "../assets/textures/top.png";
    skybox.front_tex_path  = "../assets/textures/front.png";
    skybox.back_tex_path   = "../assets/textures/back.png";
    skybox.size = 100.0f;

    // Loader
    loader.num_resources = 8;

    loader_create(&loader);
    loader_add_resource(&loader, &timer,         (Loader_Init_proc)timer_create,            (Loader_Dest_proc)timer_destroy);
    loader_add_resource(&loader, &window,        (Loader_Init_proc)window_create,           (Loader_Dest_proc)window_destroy);
    loader_add_resource(&loader, &input,         (Loader_Init_proc)input_create,            (Loader_Dest_proc)input_destroy);
    loader_add_resource(&loader, &context,       (Loader_Init_proc)context_create,          (Loader_Dest_proc)context_destroy);
    loader_add_resource(&loader, &skybox,        (Loader_Init_proc)skybox_create,           (Loader_Dest_proc)skybox_destroy);
    loader_add_resource(&loader, &skybox_shader, (Loader_Init_proc)skybox_shader_create,    (Loader_Dest_proc)skybox_shader_destroy);
    loader_add_resource(&loader, &mesh,          (Loader_Init_proc)mesh_create,             (Loader_Dest_proc)mesh_destroy);
    loader_add_resource(&loader, &mesh_shader,   (Loader_Init_proc)mesh_shader_create,      (Loader_Dest_proc)mesh_shader_destroy);
    loader_load(&loader);

    if (loader_error(&loader) != IC_NO_ERROR) return -1;

    // Update context
    context_update(&context);

    // Initialize world aspects (Light, Camera, etc.)
    camera.fov = (float)70 / (float)180 * IC_PI;

    player.type = IC_PLAYER_SUPER;

    /*
    player.aabb.extent.x = 0.1f;
    player.aabb.extent.y = 0.3f;
    player.aabb.extent.z = 0.1f;*/
    
    player_controller.player = &player;
    player_controller.xz_speed = 1.5f;
    player_controller.y_speed = 3.0f;
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
            float dt = timer_get_dt(&timer);

            // Player movement
            player_controller_update(&player_controller, &input, dt);

            camera.position = player.position;
            camera.pitch = player.pitch;
            camera.yaw = player.yaw;
        }

        context.cull_front = IC_TRUE;
        context.cull = IC_FALSE;
        context_update(&context);

        Mat4 view = camera_view_matrix(&camera);
        Mat4 projection = mat4_make_project(camera.fov, window_aspect_ratio(&window), 0.01f, 1000.0f);

        skybox_shader_bind(&skybox_shader);
        skybox_shader_set_view_projection(&skybox_shader, &view, &projection);
        skybox_shader_set_skybox(&skybox_shader, &skybox);
        skybox_render(&skybox);

        context.cull_front = IC_FALSE;
        context_update(&context);

        Mat4 mesh_transform = mat4_identity();

        mesh_shader_bind(&mesh_shader);
        mesh_shader_set_transform(&mesh_shader, &mesh_transform);
        mesh_shader_set_view(&mesh_shader, &view);
        mesh_shader_set_projection(&mesh_shader, &projection);
        mesh_render(&mesh);

        window_swap_buffers(&window);
    }

    loader_unload(&loader);
    loader_destroy(&loader);

    terminate_libs();

    return 0;
}