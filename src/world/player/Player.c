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
    player->accum_velocity.y += speed;
}

void player_controller_update(const Player_Controller *controller, const Input *input, float dt)
{
    // Euler integration
    Vec3 d_vel = vec3_scale(&controller->player->acceleration, dt);
    Vec3 d_accum_vel = vec3_scale(&controller->player->accum_acceleration, dt);
    d_vel = vec3_add(&d_vel, &d_accum_vel);
    
    controller->player->velocity = vec3_add(&controller->player->velocity, &d_vel);

    Vec3 d_pos = vec3_scale(&controller->player->velocity, dt);
    Vec3 d_accum_pos = vec3_scale(&controller->player->accum_velocity, dt);
    d_pos = vec3_add(&d_pos, &d_accum_pos);

    controller->player->position = vec3_add(&controller->player->position, &d_pos);

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

    if (input_key_down(input, controller->up_key))
    {
        player_move_up(controller->player, controller->y_speed);
    }

    if (input_key_down(input, controller->down_key))
    {
        player_move_up(controller->player, -controller->y_speed);
    }

    controller->player->yaw += input_cursor_dx(input) * controller->mouse_sensitivity;
    controller->player->pitch += input_cursor_dy(input) * controller->mouse_sensitivity;
}