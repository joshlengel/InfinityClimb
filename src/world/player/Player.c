#include"world/player/Player.h"

#include"world/Camera.h"
#include"util/Utils.h"

#include<stdlib.h>
#include<math.h>

IC_ERROR_CODE player_create(Player *dest)
{
    return IC_NO_ERROR;
}

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

Player player_load_from_file(const char *path)
{
    Player player = {0};

    IC_ERROR_CODE ec;
    String source_str = read_source(path, &ec);
    if (ec != IC_NO_ERROR)
    {
        return player;
    }

    String_View source;
    string_view_create_s(&source, &source_str, 0, UINT32_MAX);

    uint32_t num_lines;
    String_View *lines = string_view_split(&source, '\n', &num_lines);

    for (uint32_t l = 0; l < num_lines; ++l)
    {
        String_View line = lines[l];
        line = string_view_trim(&line);

        uint32_t num_parts;
        String_View *parts = string_view_split(&line, ' ', &num_parts);

        if (string_view_equals_c_str(&parts[0], "collidable"))
        {
            uint32_t num_type_parts;
            String_View *type_parts = string_view_split(&parts[1], '=', &num_type_parts);

            if (string_view_equals_c_str(&type_parts[1], "sphere"))
            {
                Sphere *sphere = malloc(sizeof(Sphere));
                
                for (uint32_t i = 2; i < num_parts; ++i)
                {
                    uint32_t num_arg_pair;
                    String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                    if (string_view_equals_c_str(&arg_pair[0], "radius"))
                    {
                        sphere->radius = strtof(arg_pair[1].c_str, NULL);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_x"))
                    {
                        player.collidable_offset.x = strtof(arg_pair[1].c_str, NULL);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_y"))
                    {
                        player.collidable_offset.y = strtof(arg_pair[1].c_str, NULL);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_z"))
                    {
                        player.collidable_offset.z = strtof(arg_pair[1].c_str, NULL);
                    }

                    free(arg_pair);
                }

                player.collidable = sphere;
                player.collidable_type = IC_COLLIDABLE_SPHERE;
            }
            else if (string_view_equals_c_str(&type_parts[1], "capsule"))
            {
                Capsule *capsule = malloc(sizeof(Capsule));
                
                for (uint32_t i = 2; i < num_parts; ++i)
                {
                    uint32_t num_arg_pair;
                    String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                    if (string_view_equals_c_str(&arg_pair[0], "radius"))
                    {
                        capsule->radius = strtof(arg_pair[1].c_str, NULL);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "body_height"))
                    {
                        capsule->body_height = strtof(arg_pair[1].c_str, NULL);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_x"))
                    {
                        player.collidable_offset.x = strtof(arg_pair[1].c_str, NULL);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_y"))
                    {
                        player.collidable_offset.y = strtof(arg_pair[1].c_str, NULL);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_z"))
                    {
                        player.collidable_offset.z = strtof(arg_pair[1].c_str, NULL);
                    }

                    free(arg_pair);
                }

                player.collidable = capsule;
                player.collidable_type = IC_COLLIDABLE_CAPSULE;
            }

            free(type_parts);
        }
        else if (string_view_equals_c_str(&parts[0], "camera"))
        {
            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (string_view_equals_c_str(&arg_pair[0], "offset_x"))
                {
                    player.cam_offset.x = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "offset_y"))
                {
                    player.cam_offset.y = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "offset_z"))
                {
                    player.cam_offset.z = strtof(arg_pair[1].c_str, NULL);
                }

                free(arg_pair);
            }
        }

        free(parts);
    }

    free(lines);
    string_destroy(&source_str);

    return player;
}

void player_update_camera(Player *player, Camera *camera)
{
    camera->position = vec3_add(&player->position, &player->cam_offset);
    camera->pitch = player->pitch;
    camera->yaw = player->yaw;
}

void player_destroy(const Player *player)
{
    free(player->collidable);
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
        controller->player->acceleration.y = -17.0f; // Gravity
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