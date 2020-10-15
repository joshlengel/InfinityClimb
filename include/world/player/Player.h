#pragma once

#include"window/Input.h"
#include"math/Vec.h"

struct _Player
{
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;

    Vec3 accum_velocity;
    Vec3 accum_acceleration;

    // AABB
    Vec3 half_extents;

    float mass;

    float pitch, yaw;
};

typedef struct _Player Player;

void player_move_forward(Player *player, float speed);
void player_move_right(Player *player, float speed);
void player_move_up(Player *player, float speed);

struct _Player_Controller
{
    IC_KEY forward_key;
    IC_KEY backward_key;
    IC_KEY left_key;
    IC_KEY right_key;

    IC_KEY up_key;
    IC_KEY down_key;

    float xz_speed;
    float y_speed;

    float mouse_sensitivity;

    Player *player;
};

typedef struct _Player_Controller Player_Controller;

typedef struct _Timer Timer;

void player_controller_update(const Player_Controller *controller, const Input *input, float dt);