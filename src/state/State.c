#include"state/State.h"

#include"util/Utils.h"

#include<stdlib.h>

IC_BOOL __state_check_exit_impl(State_Machine *state_machine)
{
    State *current = state_machine->current_state;

    if (!current) return IC_TRUE;

    if (current->exit)
    {
        current->stop_proc(current);
        State *next = current->next_state;
        
        if (next)
        {
            next->window = current->window;
            next->input = current->input;
            next->timer = current->timer;
            free(current);
            next->start_proc(next);

            timer_start(next->timer);
            
            state_machine->current_state = next;

            if (next->exit) return IC_TRUE;
        }
        else
        {
            free(current);
            state_machine->current_state = NULL;
            return IC_TRUE;
        }
    }

    return IC_FALSE;
}

void state_machine_start(State_Machine *state_machine, State *start_state)
{
    state_machine->current_state = start_state;
    start_state->start_proc(start_state);
}

void state_machine_stop(State_Machine *state_machine)
{
    if (state_machine->current_state)
    {
        state_machine->current_state->stop_proc(state_machine->current_state);
        free(state_machine->current_state);
        state_machine->current_state = NULL;
    }
}

void state_machine_update(State_Machine *state_machine)
{
    if (__state_check_exit_impl(state_machine)) return;

    state_machine->current_state->update_proc(state_machine->current_state);
}

void state_machine_render(State_Machine *state_machine)
{
    if (__state_check_exit_impl(state_machine)) return;

    state_machine->current_state->render_proc(state_machine->current_state);
}