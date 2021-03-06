#include"state/Game_State.h"
#include"state/State.h"

#include"gui/components/Component.h"
#include"gui/Gui_Shader.h"
#include"gui/Gui_Lib.h"
#include"gui/Gui_Utils.h"
#include"window/Window.h"
#include"window/Input.h"
#include"util/Utils.h"
#include"Core.h"
#include"Log.h"

#include<stdlib.h>

struct _Start_State_Data
{
    IC_BOOL init_failed;

    Gui_Panel gui_backdrop;
    Gui_Pane gui_content;
    Gui_Shader gui_shader;
};

typedef struct _Start_State_Data Start_State_Data;

void start_state_start(State *state)
{
    Start_State_Data *data = malloc(sizeof(Start_State_Data));
    log_assert(data != NULL, "Error starting start state. Out of memory");
    state->data = data;

    data->init_failed = IC_FALSE;

    data->gui_backdrop.color = color_create_hex(0x666666FF);
    data->gui_content.color = color_create_hex(0x6677DDFF);
    data->gui_content.center.x = 0.0f;
    data->gui_content.center.y = 0.0f;
    data->gui_content.fixed = IC_FALSE;

    gui_init_lib();

    IC_ERROR_CODE ec;
    if ((ec = gui_shader_create(&data->gui_shader)) != IC_NO_ERROR)
    {
        state->exit = IC_TRUE;
        data->init_failed = IC_TRUE;
        return;
    }

    window_show(state->window);
    timer_start(state->timer);
}

void start_state_stop(State *state)
{
    if (((Start_State_Data*)state->data)->init_failed)
    {
        state->next_state = NULL;
    }
    else
    {
        state->next_state = malloc(sizeof(State));
        state->next_state->exit = IC_FALSE;
        state->next_state->start_proc  = game_state_start;
        state->next_state->stop_proc   = game_state_stop;
        state->next_state->update_proc = game_state_update;
        state->next_state->render_proc = game_state_render;
    }

    free(state->data);
}

void start_state_update(State *state)
{
    if (input_key_pressed(state->input, IC_KEY_SPACE))
    {
        state->exit = IC_TRUE;
    }

    Start_State_Data *data = (Start_State_Data*)state->data;

    data->gui_content.extents.x = gui_width_to_extent(state->window->width - 30 * 2, state->window); // 30 pixels from either edge
    data->gui_content.extents.y = gui_height_to_extent(state->window->height - 30 * 2, state->window); // 30 pixels from either edge
}

void start_state_render(State *state)
{
    Start_State_Data *data = (Start_State_Data*)state->data;
    gui_panel_render(&data->gui_backdrop, &data->gui_shader);
    gui_pane_render(&data->gui_content, &data->gui_shader, state->window);
}