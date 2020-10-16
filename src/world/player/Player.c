#include"world/player/Player.h"

#include"Utils.h"

#include<math.h>

void player_move_forward(Player *player, float speed)
{
    player->accum_velocity.x += sinf(player->yaw) * speed;
    player->accum_velocity.z += cosf(player->yaw) * speed;
}

void player_move_right(Player *player, float speed)
{
    player->accum_velocity.x += cosf(player->yaw) * speed;
    player->accum_velocity.z -= sinf(player->yaw) * speed;
}

void player_move_up(Player *player, float speed)
{
    if (player->type == IC_PLAYER_SUPER)
    {
        player->accum_velocity.y += speed;
    }
    else
    {
        player->velocity.y += speed;
    }
}

void player_controller_update(const Player_Controller *controller, const Input *input, float dt)
{
    controller->player->accum_velocity.x = 0;
    controller->player->accum_velocity.y = 0;
    controller->player->accum_velocity.z = 0;
    controller->player->accum_acceleration.x = 0;
    controller->player->accum_acceleration.y = 0;
    controller->player->accum_acceleration.z = 0;

    if (input_key_down(input, controller->forward_key))
    {
        player_move_forward(controller->player, controller->xz_speed);
    }

    if (input_key_down(input, controller->backward_key))
    {
        player_move_forward(controller->player, -controller->xz_speed);
    }

    if (input_key_down(input, controller->right_key))
    {
        player_move_right(controller->player, controller->xz_speed);
    }

    if (input_key_down(input, controller->left_key))
    {
        player_move_right(controller->player, -controller->xz_speed);
    }

    if (controller->player->type == IC_PLAYER_SUPER)
    {
        if (input_key_down(input, controller->up_key))
        {
            player_move_up(controller->player, controller->y_speed);
        }

        if (input_key_down(input, controller->down_key))
        {
            player_move_up(controller->player, -controller->y_speed);
        }
    }
    else
    {
        if (input_key_pressed(input, controller->up_key))
        {
            player_move_up(controller->player, controller->y_speed);
        }
    }

    controller->player->yaw += input_cursor_dx(input) * controller->mouse_sensitivity;
    controller->player->pitch += input_cursor_dy(input) * controller->mouse_sensitivity;

    if (controller->player->type == IC_PLAYER_NORMAL)
    {
        controller->player->accum_acceleration.y -= 9.81f; // Gravity
    }

    // Euler integration
    Vec3 d_vel = vec3_scale(&controller->player->acceleration, dt);
    Vec3 d_accum_vel = vec3_scale(&controller->player->accum_acceleration, dt);
    d_vel = vec3_add(&d_vel, &d_accum_vel);

    controller->player->velocity = vec3_add(&controller->player->velocity, &d_vel);
}