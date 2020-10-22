#include"state/State.h"

#include"window/Context.h"
#include"window/Input.h"
#include"world/model/mesh/Mesh_Shader.h"
#include"world/model/skybox/Skybox_Shader.h"
#include"world/player/Player.h"
#include"world/Camera.h"
#include"world/Level.h"
#include"util/math/Vec.h"
#include"util/Color.h"
#include"util/Utils.h"

#include<stdlib.h>

struct _Game_State_Data
{
    Loader loader;
    Context context;

    Camera camera;
    Player_Controller player_controller;

    Level level;
    Mesh_Shader mesh_shader;
    Skybox_Shader skybox_shader;

    Vec3 light_position;
    Color sky_color;

    Color floor_color;
    Color stair_color;
    Color box_color;
};

typedef struct _Game_State_Data Game_State_Data;

void game_state_start(State *state)
{
    Game_State_Data *data = malloc(sizeof(Game_State_Data));
    state->data = data;

    // Context
    data->sky_color = color_create_hex(0x87CEEBFF);
    
    data->context.background_color = &data->sky_color;
    data->context.clear_color = IC_TRUE;
    data->context.clear_depth = IC_TRUE;
    data->context.cull = IC_TRUE;
    data->context.cull_front = IC_FALSE;

    // Level
    data->level = level_load_from_file("../../assets/components/level1.iclevel");
    data->level.player.type = IC_PLAYER_NORMAL;
    data->level.mesh_shader = &data->mesh_shader;
    data->level.skybox_shader = &data->skybox_shader;

    data->camera.fov = (float)70 / (float)180 * (float)IC_PI;

    data->player_controller.player = &data->level.player;
    data->player_controller.xz_speed = 0.5f;
    data->player_controller.y_speed = 6.0f;
    data->player_controller.mouse_sensitivity = 0.001f;

    data->player_controller.forward_key  = IC_KEY_W;
    data->player_controller.backward_key = IC_KEY_S;
    data->player_controller.right_key    = IC_KEY_D;
    data->player_controller.left_key     = IC_KEY_A;
    data->player_controller.up_key       = IC_KEY_SPACE;
    data->player_controller.down_key     = IC_KEY_SHIFT;

    // Loader
    data->loader.num_resources = 4;

    loader_create(&data->loader);
    loader_add_resource(&data->loader, &data->context,       (Loader_Init_proc)context_create,          (Loader_Dest_proc)context_destroy);
    loader_add_resource(&data->loader, &data->skybox_shader, (Loader_Init_proc)skybox_shader_create,    (Loader_Dest_proc)skybox_shader_destroy);
    loader_add_resource(&data->loader, &data->mesh_shader,   (Loader_Init_proc)mesh_shader_create,      (Loader_Dest_proc)mesh_shader_destroy);
    loader_add_resource(&data->loader, &data->level,         (Loader_Init_proc)level_create,            (Loader_Dest_proc)level_destroy);
    loader_load(&data->loader);

    if (loader_error(&data->loader) != IC_NO_ERROR)
    {
        state->exit = IC_TRUE;
        return;
    }

    // Update context
    context_update(&data->context);

    // Start
    input_disable_cursor(state->input);
}

void game_state_stop(State *state)
{
    Game_State_Data *data = (Game_State_Data*)state->data;

    loader_unload(&data->loader);
    loader_destroy(&data->loader);
    free(data);
}

void game_state_update(State *state)
{
    Game_State_Data *data = (Game_State_Data*)state->data;

    // Check to see if user pressed escape to bring cursor back
    if (input_key_pressed(state->input, IC_KEY_ESCAPE))
    {
        input_toggle_cursor(state->input);
    }

    if (!input_cursor_enabled(state->input))
    {
        float dt = timer_get_dt(state->timer);

        // Player movement
        player_controller_update(&data->player_controller, state->input, dt);
        level_update(&data->level, &data->camera, dt);
    }
}

void game_state_render(State *state)
{
    Game_State_Data *data = (Game_State_Data*)state->data;

    // Clear screen
    context_clear(&data->context);

    data->context.cull_front = IC_TRUE;
    context_update(&data->context);
    level_render(&data->level, state->window, &data->camera);
}