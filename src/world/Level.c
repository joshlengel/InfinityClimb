#include"world/Level.h"

#include"world/Camera.h"
#include"world/Physics.h"
#include"world/rect/Rect.h"
#include"world/rect/Rect_Shader.h"
#include"world/player/Player.h"
#include"window/Window.h"

#include"math/Color.h"

#include<stdlib.h>
#include<assert.h>

struct _Level_Data
{
    const Rect **rects;
    uint32_t num_rects, rect_index;
};

IC_ERROR_CODE level_create(Level *dest)
{
    Level_Data *data = malloc(sizeof(Level_Data));
    dest->data = data;
    data->rects = malloc(sizeof(Rect*) * dest->num_rects);
    data->num_rects = dest->num_rects;
    data->rect_index = 0;

    dest->camera->position = dest->start_position;
    dest->camera->pitch = dest->start_pitch;
    dest->camera->yaw = dest->start_yaw;

    dest->player->position = dest->start_position;
    dest->player->pitch = dest->start_pitch;
    dest->player->yaw = dest->start_yaw;

    return IC_NO_ERROR;
}

void level_destroy(const Level *level)
{
    free(level->data->rects);
    free(level->data);
}

void level_add_rect(const Level *level, const Rect *rect)
{
    assert(level->data->rect_index < level->data->num_rects);

    *(level->data->rects + level->data->rect_index++) = rect;
}

void level_update(const Level *level, float dt)
{
    level->player->aabb.center = level->player->position;

    float time_left = dt;
    while (time_left > 0)
    {
        Vec3 sum_vel = vec3_add(&level->player->velocity, &level->player->accum_velocity);

        Collision_Result min_result;
        min_result.time_simulated = time_left;
        min_result.obj1_displacement = vec3_scale(&sum_vel, time_left);
        min_result.obj1_vel = level->player->velocity;
        min_result.obj1_accum_vel = level->player->accum_velocity;

        const Rect **itr = level->data->rects;
        for (uint32_t i = 0; i < level->data->num_rects; ++i)
        {
            const Rect *rect = *(itr++);

            Collision_Result result = physics_collide(&level->player->aabb, &level->player->velocity, &level->player->accum_velocity, &rect->aabb, time_left);
            if (result.time_simulated < min_result.time_simulated)
            {
                min_result = result;
            }
        }

        level->player->position = vec3_add(&level->player->position, &min_result.obj1_displacement);
        level->player->velocity = min_result.obj1_vel;
        level->player->accum_velocity = min_result.obj1_accum_vel;
        time_left -= min_result.time_simulated;
    }

    // First person camera
    level->camera->position = level->player->position;
    level->camera->pitch = level->player->pitch;
    level->camera->yaw = level->player->yaw;
}

void level_render(const Level *level)
{
    // TODO: Create 'renderable' to automaticially retrieve data for shader uniforms
    rect_shader_bind(level->rect_shader);

    Mat4 view = camera_view_matrix(level->camera);
    Mat4 projection = mat4_make_project(level->camera->fov, window_aspect_ratio(level->window), 0.01f, 100.0f);

    rect_shader_set_view_projection(level->rect_shader, &view, &projection);

    Color rect_color = color_create_hex(0x666666FF);
    rect_shader_set_color(level->rect_shader, &rect_color);
    rect_shader_set_light(level->rect_shader, &level->light_dir);
    
    const Rect **itr = level->data->rects;
    for (uint32_t i = 0; i < level->data->num_rects; ++i)
    {
        const Rect *rect = *(itr++);

        rect_shader_set_rect(level->rect_shader, rect);

        rect_render(rect);
    }
}