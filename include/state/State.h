#pragma once

#include"Core.h"

typedef struct _State State;
typedef void(*State_Start_Proc)(State *state);
typedef void(*State_Stop_Proc)(State *state);
typedef void(*State_Update_Proc)(State *state);
typedef void(*State_Render_Proc)(State *state);

typedef struct _Window Window;
typedef struct _Input Input;
typedef struct _Timer Timer;

struct _State
{
    State_Start_Proc start_proc;
    State_Stop_Proc stop_proc;
    State_Update_Proc update_proc;
    State_Render_Proc render_proc;

    State *next_state;

    Window *window;
    Input *input;
    Timer *timer;
    void *data;

    IC_BOOL exit;
};

struct _State_Machine
{
    State *current_state;
};

typedef struct _State_Machine State_Machine;

void state_machine_start(State_Machine *state_machine, State *start_state);
void state_machine_stop(State_Machine *state_machine);
void state_machine_update(State_Machine *state_machine);
void state_machine_render(State_Machine *state_machine);