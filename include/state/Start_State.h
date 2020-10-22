#pragma once

typedef struct _State State;

void start_state_start(State *state);
void start_state_stop(State *state);
void start_state_update(State *state);
void start_state_render(State *state);