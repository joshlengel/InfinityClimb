#include"state/Game_State.h"
#include"state/State.h"

#include"window/Window.h"
#include"window/Input.h"
#include"util/Utils.h"
#include"Core.h"

#include<stdlib.h>

typedef struct _Start_State_Data Start_State_Data;

void start_state_start(State *state)
{
    window_show(state->window);
    timer_start(state->timer);
}

void start_state_stop(State *state)
{}

void start_state_update(State *state)
{
    if (input_key_pressed(state->input, IC_KEY_SPACE))
    {
        state->exit = IC_TRUE;
        state->next_state = malloc(sizeof(State));
        state->next_state->exit = IC_FALSE;
        state->next_state->start_proc  = game_state_start;
        state->next_state->stop_proc   = game_state_stop;
        state->next_state->update_proc = game_state_update;
        state->next_state->render_proc = game_state_render;
    }
}

void start_state_render(State *state)
{}