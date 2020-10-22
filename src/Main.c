#define _CRT_SECURE_NO_WARNINGS

#include"util/Utils.h"
#include"util/Color.h"
#include"util/math/Mat.h"
#include"window/Window.h"
#include"window/Input.h"
#include"state/State.h"
#include"state/Start_State.h"
#include"Libs.h"
#include"Log.h"
#include"Core.h"

#include<stdlib.h>

Window window;
Input input;
Timer timer;

Loader window_loader;

IC_ERROR_CODE init()
{
    IC_ERROR_CODE ec;

    // Load libraries
    if ((ec = load_libs()) != IC_NO_ERROR)
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
    if (init() != IC_NO_ERROR)
    {
        FILE *init_log = fopen("../../logs/init_log.txt", "w");
        if (!init_log)
        {
            dump_log(init_log);
            fclose(init_log);
        }
        loader_unload(&window_loader);
        terminate_libs();
        return -1;
    }
    else
    {
        remove("../../logs/init_log.txt");
    }

    State *start_state = malloc(sizeof(State));
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

    loader_unload(&window_loader);
    loader_destroy(&window_loader);

    terminate_libs();

    return 0;
}