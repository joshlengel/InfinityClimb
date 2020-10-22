#pragma once

typedef struct _State State;

void game_state_start(State *state);
void game_state_stop(State *state);
void game_state_update(State *state);
void game_state_render(State *state);