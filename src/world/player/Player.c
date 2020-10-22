#include"world/player/Player.h"

#include"window/Input.h"
#include"world/Camera.h"
#include"util/Utils.h"
#include"Log.h"

#include<stdlib.h>
#include<math.h>

#ifdef IC_DEBUG
const char *SPHERE_OPTION_NAMES[4] =
{
    "radius", "offset_x", "offset_y", "offset_z"
};

const char *CAPSULE_OPTION_NAMES[5] =
{
    "radius", "body_height", "offset_x", "offset_y", "offset_z"
};

const char *CAMERA_OPTION_NAMES[3] =
{
    "offset_x", "offset_y", "offset_z"
};

const char *PLAYER_IDENTIFIER_NAMES[2] =
{
    "collidable", "camera"
};

void __player_check_required_options_impl(IC_OPTIONS_32 options, uint8_t num_options, const char *part_name, const char **option_names, uint32_t line, const char *path, IC_ERROR_CODE *error_code)
{
    for (uint8_t i = 0; i < num_options; ++i)
    {
        if (!IC_DEBUG_OPTION_IS_SET(options, i))
        {
            log_trace("Error on line %u at '%s': Option '%s' of %s must be set", line, path, option_names[i], part_name);
            if (error_code) *error_code = IC_PLAYER_INCOMPLETE_ERROR;
        }
    }
}

#endif // IC_DEBUG

#ifdef IC_DEBUG
    #define __check_required_options(options, num_options, part_name, option_names, line, path, error_code) __player_check_required_options_impl(options, num_options, part_name, option_names, line, path, error_code)
#else
    #define __check_required_options(options, num_options, part_name, option_names, line, path, error_code)
#endif // IC_DEBUG

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

Player player_load_from_file(const char *path, IC_ERROR_CODE *error_code)
{
    if (error_code) *error_code = IC_NO_ERROR;

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

    IC_DEBUG_OPTIONS_32(player_options)

    for (uint32_t l = 0; l < num_lines; ++l)
    {
        String_View line = lines[l];
        line = string_view_trim(&line);

        uint32_t num_parts;
        String_View *parts = string_view_split(&line, ' ', &num_parts);

        if (num_parts == 0) // Not really an error
        {
            free(parts);
            log_trace("Warning on line %u at '%s': Empty line with whitespace found. This is not critical, but leads to inefficiencies", l, path);
            if (error_code) *error_code = IC_PLAYER_WHITESPACE_INEFFICIENCY;
            continue;
        }

        if (string_view_equals_c_str(&parts[0], "collidable"))
        {
            if (num_parts == 1)
            {
                free(parts);
                log_trace("Warning on line %u at '%s': Missing all options of collidable", l, path);
                if (error_code) *error_code = IC_PLAYER_INCOMPLETE_ERROR;
                continue;
            }

            uint32_t num_type_parts;
            String_View *type_parts = string_view_split(&parts[1], '=', &num_type_parts);

            if (string_view_equals_c_str(&type_parts[1], "sphere"))
            {
                IC_DEBUG_OPTIONS_32(options);

                Sphere *sphere = malloc(sizeof(Sphere));
                
                for (uint32_t i = 2; i < num_parts; ++i)
                {
                    uint32_t num_arg_pair;
                    String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                    if (num_arg_pair == 0) // Incorrect syntax
                    {
                        log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", l, path, parts[i].length, parts[i].c_str);
                        free(arg_pair);
                        if (error_code) *error_code = IC_PLAYER_SYNTAX_ERROR;
                        continue;
                    }

                    if (string_view_equals_c_str(&arg_pair[0], "radius"))
                    {
                        sphere->radius = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 0);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_x"))
                    {
                        player.collidable_offset.x = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 1);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_y"))
                    {
                        player.collidable_offset.y = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 2);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_z"))
                    {
                        player.collidable_offset.z = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 3);
                    }
                    else
                    {
                        log_trace("Warning on line %u at '%s': Unrecognized option '%.*s'", l, path, arg_pair[0].length, arg_pair[0].c_str);
                        if (error_code) *error_code = IC_PLAYER_SYNTAX_ERROR;
                    }

                    free(arg_pair);
                }

                __check_required_options(options, 4, "sphere", SPHERE_OPTION_NAMES, l, path, error_code);

                player.collidable = sphere;
                player.collidable_type = IC_COLLIDABLE_SPHERE;

                IC_DEBUG_OPTION_SET(player_options, 0);
            }
            else if (string_view_equals_c_str(&type_parts[1], "capsule"))
            {
                IC_DEBUG_OPTIONS_32(options)

                Capsule *capsule = malloc(sizeof(Capsule));
                
                for (uint32_t i = 2; i < num_parts; ++i)
                {
                    uint32_t num_arg_pair;
                    String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                    if (num_arg_pair == 0) // Incorrect syntax
                    {
                        log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", l, path, parts[i].length, parts[i].c_str);
                        free(arg_pair);
                        if (error_code) *error_code = IC_PLAYER_SYNTAX_ERROR;
                        continue;
                    }

                    if (string_view_equals_c_str(&arg_pair[0], "radius"))
                    {
                        capsule->radius = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 0);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "body_height"))
                    {
                        capsule->body_height = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 1);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_x"))
                    {
                        player.collidable_offset.x = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 2);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_y"))
                    {
                        player.collidable_offset.y = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 3);
                    }
                    else if (string_view_equals_c_str(&arg_pair[0], "offset_z"))
                    {
                        player.collidable_offset.z = strtof(arg_pair[1].c_str, NULL);
                        IC_DEBUG_OPTION_SET(options, 4);
                    }
                    else
                    {
                        log_trace("Warning on line %u at '%s': Unrecognized option '%.*s'", l, path, arg_pair[0].length, arg_pair[0].c_str);
                        if (error_code) *error_code = IC_PLAYER_SYNTAX_ERROR;
                    }

                    free(arg_pair);
                }

                __check_required_options(options, 5, "capsule", CAPSULE_OPTION_NAMES, l, path, error_code);

                player.collidable = capsule;
                player.collidable_type = IC_COLLIDABLE_CAPSULE;

                IC_DEBUG_OPTION_SET(player_options, 0);
            }

            free(type_parts);
        }
        else if (string_view_equals_c_str(&parts[0], "camera"))
        {
            IC_DEBUG_OPTIONS_32(options)

            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (num_arg_pair == 0) // Incorrect syntax
                {
                    log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", l, path, parts[i].length, parts[i].c_str);
                    free(arg_pair);
                    if (error_code) *error_code = IC_PLAYER_SYNTAX_ERROR;
                    continue;
                }

                if (string_view_equals_c_str(&arg_pair[0], "offset_x"))
                {
                    player.cam_offset.x = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 0);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "offset_y"))
                {
                    player.cam_offset.y = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 1);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "offset_z"))
                {
                    player.cam_offset.z = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 2);
                }
                else
                {
                    log_trace("Warning on line %u at '%s': Unrecognized option '%.*s'", l, path, arg_pair[0].length, arg_pair[0].c_str);
                    if (error_code) *error_code = IC_PLAYER_SYNTAX_ERROR;
                }

                free(arg_pair);
            }

            __check_required_options(options, 3, "camera", CAMERA_OPTION_NAMES, l, path, error_code);

            IC_DEBUG_OPTION_SET(player_options, 1);
        }

        free(parts);
    }

    free(lines);
    string_destroy(&source_str);

#ifdef IC_DEBUG
    for (uint8_t i = 0; i < 2; ++i)
    {
        if (!IC_DEBUG_OPTION_IS_SET(player_options, i))
        {
            log_trace("Error in player file at '%s': Missing options of identifier '%s'", path, PLAYER_IDENTIFIER_NAMES[i]);
            if (error_code) *error_code = IC_PLAYER_INCOMPLETE_ERROR;
        }
    }
#endif // IC_DEBUG

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