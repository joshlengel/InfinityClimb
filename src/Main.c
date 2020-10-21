#include"util/Utils.h"
#include"util/Color.h"
#include"util/math/Mat.h"
#include"window/Window.h"
#include"window/Input.h"
#include"window/Context.h"
#include"world/Camera.h"
#include"world/model/Model.h"
#include"world/model/skybox/Skybox.h"
#include"world/model/skybox/Skybox_Shader.h"
#include"world/model/mesh/Mesh.h"
#include"world/model/mesh/Mesh_Shader.h"
#include"world/model/physics/Collidable.h"
#include"world/player/Player.h"
#include"Libs.h"
#include"Log.h"
#include"Core.h"

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

Mesh floor_mesh;
Model floor_model;
Mesh stair_mesh;
Model stair_model;
Mesh box_mesh;
Model box_model;
Mesh_Shader mesh_shader;

Skybox skybox;
Skybox_Shader skybox_shader;

Vec3 LIGHT_POSITION = { -30.0f, 40.0f, 20.0f };
Color SKY_COLOR;

Color floor_color;
Color stair_color;
Color box_color;

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

    // Models
    floor_mesh = mesh_load_from_obj("../../assets/models/floor.obj");
    stair_mesh = mesh_load_from_obj("../../assets/models/stairs.obj");
    box_mesh = mesh_load_from_obj("../../assets/models/box.obj");

    floor_model.mesh = &floor_mesh;
    floor_model.scale.x = 5.0f;
    floor_model.scale.y = 1.0f;
    floor_model.scale.z = 5.0f;

    stair_model.mesh = &stair_mesh;
    stair_model.position.x = 0.25f;
    stair_model.position.y = 0.6f;
    stair_model.position.z = 0.5f;
    stair_model.scale.x = 0.4f;
    stair_model.scale.y = 0.4f;
    stair_model.scale.z = 0.4f;

    box_model.mesh = &box_mesh;
    box_model.position.x = -0.75f;
    box_model.position.y = 0.6f;
    box_model.position.z = 0.1f;
    box_model.scale.x = 0.6f;
    box_model.scale.y = 0.4f;
    box_model.scale.z = 0.8f;

    floor_color = color_create_hex(0x888888FF);
    stair_color = color_create_hex(0x777777FF);
    box_color = color_create_hex(0x777777FF);

    // Skybox
    skybox.left_tex_path   = "../../assets/textures/left.png";
    skybox.right_tex_path  = "../../assets/textures/right.png";
    skybox.bottom_tex_path = "../../assets/textures/bottom.png";
    skybox.top_tex_path    = "../../assets/textures/top.png";
    skybox.front_tex_path  = "../../assets/textures/front.png";
    skybox.back_tex_path   = "../../assets/textures/back.png";
    skybox.size = 100.0f;

    // Player
    player = player_load_from_file("../../assets/components/player1.icplayer");

    player.type = IC_PLAYER_NORMAL;
    player.position.x = 0.0f;
    player.position.y = 2.0f;
    player.position.z = 0.0f;
    player.pitch = 0.0f;
    player.yaw = 0.0f;

    camera.fov = (float)70 / (float)180 * IC_PI;

    player_controller.player = &player;
    player_controller.xz_speed = 0.5f;
    player_controller.y_speed = 6.0f;
    player_controller.mouse_sensitivity = 0.001f;

    player_controller.forward_key  = IC_KEY_W;
    player_controller.backward_key = IC_KEY_S;
    player_controller.right_key    = IC_KEY_D;
    player_controller.left_key     = IC_KEY_A;
    player_controller.up_key       = IC_KEY_SPACE;
    player_controller.down_key     = IC_KEY_SHIFT;

    // Loader
    loader.num_resources = 11;

    loader_create(&loader);
    loader_add_resource(&loader, &timer,         (Loader_Init_proc)timer_create,            (Loader_Dest_proc)timer_destroy);
    loader_add_resource(&loader, &window,        (Loader_Init_proc)window_create,           (Loader_Dest_proc)window_destroy);
    loader_add_resource(&loader, &input,         (Loader_Init_proc)input_create,            (Loader_Dest_proc)input_destroy);
    loader_add_resource(&loader, &context,       (Loader_Init_proc)context_create,          (Loader_Dest_proc)context_destroy);
    loader_add_resource(&loader, &skybox,        (Loader_Init_proc)skybox_create,           (Loader_Dest_proc)skybox_destroy);
    loader_add_resource(&loader, &skybox_shader, (Loader_Init_proc)skybox_shader_create,    (Loader_Dest_proc)skybox_shader_destroy);
    loader_add_resource(&loader, &floor_mesh,    (Loader_Init_proc)mesh_create,             (Loader_Dest_proc)mesh_destroy);
    loader_add_resource(&loader, &stair_mesh,    (Loader_Init_proc)mesh_create,             (Loader_Dest_proc)mesh_destroy);
    loader_add_resource(&loader, &box_mesh,      (Loader_Init_proc)mesh_create,             (Loader_Dest_proc)mesh_destroy);
    loader_add_resource(&loader, &mesh_shader,   (Loader_Init_proc)mesh_shader_create,      (Loader_Dest_proc)mesh_shader_destroy);
    loader_add_resource(&loader, &player,        (Loader_Init_proc)player_create,           (Loader_Dest_proc)player_destroy);
    loader_load(&loader);

    if (loader_error(&loader) != IC_NO_ERROR) return -1;

    // Update context
    context_update(&context);

    return 0;
}

int main(int argc, char **argv)
{
    if (init() != 0)
    {
        FILE *init_log = fopen("../../logs/init_log.txt", "w");
        dump_log(init_log);
        fclose(init_log);

        terminate_libs();
        return -1;
    }
    else
    {
        remove("../../logs/init_log.txt");
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
            player.hit_ground = IC_FALSE;

            // Collision
            Collision_Result result;

            typedef Collision_Result (*Collider)(void *capsule, const Model *model, const Player *player, float dt);
            Collider collide;

            switch (player.collidable_type)
            {
                case IC_COLLIDABLE_SPHERE: collide = (Collider)collide_sphere_with_static; break;
                case IC_COLLIDABLE_CAPSULE: collide = (Collider)collide_capsule_with_static; break;
                default: continue;
            }

            result = collide(player.collidable, &floor_model, &player, dt);
            player.position = vec3_add(&player.position, &result.displacement);
            player.velocity = result.res_velocity;
            player.hit_ground |= result.hit_ground;

            result = collide(player.collidable, &stair_model, &player, dt);
            player.position = vec3_add(&player.position, &result.displacement);
            player.velocity = result.res_velocity;
            player.hit_ground |= result.hit_ground;

            result = collide(player.collidable, &box_model, &player, dt);
            player.position = vec3_add(&player.position, &result.displacement);
            player.velocity = result.res_velocity;
            player.hit_ground |= result.hit_ground;

            player_update_camera(&player, &camera);
        }

        context.cull_front = IC_TRUE;
        context_update(&context);

        Mat4 view = camera_view_matrix(&camera);
        Mat4 projection = mat4_make_project(camera.fov, window_aspect_ratio(&window), 0.01f, 1000.0f);

        skybox_shader_bind(&skybox_shader);
        skybox_shader_set_view_projection(&skybox_shader, &view, &projection);
        skybox_shader_set_skybox(&skybox_shader, &skybox);
        skybox_render(&skybox);

        mesh_shader_bind(&mesh_shader);
        mesh_shader_set_view(&mesh_shader, &view);
        mesh_shader_set_projection(&mesh_shader, &projection);

        Mat4 model_transform;
        
        model_transform = model_transform_matrix(&floor_model);
        mesh_shader_set_transform(&mesh_shader, &model_transform);
        mesh_shader_set_color(&mesh_shader, &floor_color);
        model_render(&floor_model);

        model_transform = model_transform_matrix(&stair_model);
        mesh_shader_set_transform(&mesh_shader, &model_transform);
        mesh_shader_set_color(&mesh_shader, &stair_color);
        model_render(&stair_model);

        model_transform = model_transform_matrix(&box_model);
        mesh_shader_set_transform(&mesh_shader, &model_transform);
        mesh_shader_set_color(&mesh_shader, &box_color);
        model_render(&box_model);

        window_swap_buffers(&window);
    }

    loader_unload(&loader);
    loader_destroy(&loader);

    terminate_libs();

    return 0;
}