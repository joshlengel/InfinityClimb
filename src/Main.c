#define _CRT_SECURE_NO_WARNINGS

#include"window/Window.h"
#include"window/Input.h"
#include"state/State.h"
#include"state/Start_State.h"
#include"util/Utils.h"
#include"util/String.h"
#include"Libs.h"
#include"Log.h"
#include"Core.h"
#include"IC_Config.h"

#include<stdio.h>
#include<stdlib.h>

Window window;
Input input;
Timer timer;

Loader window_loader;

IC_ERROR_CODE init()
{
    IC_ERROR_CODE ec;

    // Load libraries
    ec = load_libs();
    if (ec != IC_NO_ERROR)
    {
        log_trace("Error loading libs. Error code: %i", ec);
        return ec;
    }

    // Timer
    timer.fps = 120;

    // Window
    window.width = 800;
    window.height = 600;
    window.title = "Hello World!";

    input.window = &window;

    window_loader.num_resources = 3;

    loader_create(&window_loader);
    loader_add_resource(&window_loader, &window, (Loader_Init_proc)window_create, (Loader_Dest_proc)window_destroy);
    loader_add_resource(&window_loader, &input,  (Loader_Init_proc)input_create,  (Loader_Dest_proc)input_destroy);
    loader_add_resource(&window_loader, &timer,  (Loader_Init_proc)timer_create,  (Loader_Dest_proc)timer_destroy);
    loader_load(&window_loader);

    return loader_error(&window_loader);
}

int main(int argc, char **argv)
{
    init();

#ifdef IC_DEBUG
    String_View work_dir;
    string_view_create_c_str(&work_dir, IC_WORKING_DIRECTORY, 0, UINT32_MAX);
#endif // IC_DEBUG

#ifdef IC_DEBUG
    String_View init_rel_path;
    string_view_create_c_str(&init_rel_path, "logs/init_log.txt", 0, UINT32_MAX);

    String init_path = string_concat_sv(&work_dir, &init_rel_path);
    FILE *init_log = fopen(init_path.c_str, "w");
    string_destroy(&init_path);

    if (init_log)
    {
        dump_log(init_log);
        flush_log();
        fclose(init_log);
    }
#endif // IC_DEBUG

    State *start_state = malloc(sizeof(State));
    log_assert(start_state != NULL, "Error starting InfinityClimb. Out of memory");
    start_state->exit = IC_FALSE;
    start_state->start_proc = start_state_start;
    start_state->stop_proc = start_state_stop;
    start_state->update_proc = start_state_update;
    start_state->render_proc = start_state_render;
    start_state->window = &window;
    start_state->input = &input;
    start_state->timer = &timer;

    State_Machine state_machine;

    state_machine_start(&state_machine, start_state);

    while(!window_should_close(&window) && state_machine.current_state != NULL)
    {
        if (!timer_should_update(&timer)) continue;

        // Update input
        input_update(&input);
        window_poll_events();
        
        state_machine_update(&state_machine);
        state_machine_render(&state_machine);

        window_swap_buffers(&window);
    }

#ifdef IC_DEBUG
    String_View game_rel_path;
    string_view_create_c_str(&game_rel_path, "logs/game_log.txt", 0, UINT32_MAX);

    String game_path = string_concat_sv(&work_dir, &game_rel_path);
    FILE *game_log = fopen(game_path.c_str, "w");
    string_destroy(&game_path);
    
    if (game_log)
    {
        dump_log(game_log);
        flush_log();
        fclose(game_log);
    }
#endif // IC_DEBUG

    loader_unload(&window_loader);
    loader_destroy(&window_loader);

    terminate_libs();

    return 0;
}