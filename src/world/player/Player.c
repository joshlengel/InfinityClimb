#include"world/player/Player.h"

#include"util/Utils.h"

#include<math.h>

void player_move_forward(Player *player, float speed)
{
    player->velocity.x += sinf(player->yaw) * speed;
    player->velocity.z += cosf(player->yaw) * speed;
}

void player_move_right(Player *player, float speed)
{
    player->velocity.x += cosf(player->yaw) * speed;
    player->velocity.z -= sinf(player->yaw) * speed;
}

void player_move_up(Player *player, float speed)
{
    player->velocity.y += speed;
}

void player_controller_update(const Player_Controller *controller, const Input *input, float dt)
{
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
        controller->player->acceleration.y = -20.0f; // Gravity
    }

    // Euler integration
    Vec3 d_vel = vec3_scale(&controller->player->acceleration, dt);
    controller->player->velocity = vec3_add(&controller->player->velocity, &d_vel);

    Vec3 d_pos = vec3_scale(&controller->player->velocity, dt);
    controller->player->position = vec3_add(&controller->player->position, &d_pos);

    if (controller->player->type == IC_PLAYER_NORMAL)
    {
        if (controller->player->hit_ground)
        {
            controller->player->velocity = vec3_scale(&controller->player->velocity, 0.9f); // High drag
        }
        else
        {
            controller->player->velocity.x = controller->player->velocity.x * 0.92f; // Low drag
            controller->player->velocity.z = controller->player->velocity.z * 0.92f; // Low drag
        }
    }
    else
    {
        controller->player->velocity = vec3_scale(&controller->player->velocity, 0.8f); // High drag
    }

    if (vec3_length_sqr(&controller->player->velocity) < 0.01f * 0.01f)
    {
        controller->player->velocity.x = 0.0f;
        controller->player->velocity.y = 0.0f;
        controller->player->velocity.z = 0.0f;
    }
}