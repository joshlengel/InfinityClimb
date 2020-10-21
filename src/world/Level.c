#include"world/Level.h"

#include"world/Camera.h"
#include"window/Window.h"

#include"world/model/mesh/Mesh.h"
#include"world/model/mesh/Mesh_Shader.h"
#include"world/model/skybox/Skybox.h"
#include"world/model/skybox/Skybox_Shader.h"
#include"world/model/Model.h"

#include"util/Utils.h"
#include"util/Vector.h"

#include<stdlib.h>

const char *PREFIX = "../../";

Level level_load_from_file(const char *path)
{
    Level res = {0};
    Vec3 player_start_pos;
    float player_start_pitch;
    float player_start_yaw;

    IC_ERROR_CODE ec;
    String source_str = read_source(path, &ec);
    if (ec != IC_NO_ERROR)
    {
        return res;
    }

    String_View source;
    string_view_create_s(&source, &source_str, 0, UINT32_MAX);

    Vector meshes = {.elem_size=sizeof(Mesh)};
    Vector models = {.elem_size=sizeof(Model)};
    vector_create(&meshes, 5);
    vector_create(&models, 5);

    uint32_t num_lines;
    String_View *lines = string_view_split(&source, '\n', &num_lines);

    for (uint32_t l = 0; l < num_lines; ++l)
    {
        String_View line = lines[l];
        line = string_view_trim(&line);

        uint32_t num_parts;
        String_View *parts = string_view_split(&line, ' ', &num_parts);

        if (string_view_equals_c_str(&parts[0], "player"))
        {
            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (string_view_equals_c_str(&arg_pair[0], "path"))
                {
                    String_View prefix;
                    string_view_create_c_str(&prefix, PREFIX, 0, UINT32_MAX); // Executable is two layers deep

                    String path = string_concat_sv(&prefix, &arg_pair[1]);

                    res.player = player_load_from_file(path.c_str);

                    string_destroy(&path);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_x"))
                {
                    player_start_pos.x = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_y"))
                {
                    player_start_pos.y = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_z"))
                {
                    player_start_pos.z = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_pitch"))
                {
                    player_start_pitch = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_yaw"))
                {
                    player_start_yaw = strtof(arg_pair[1].c_str, NULL);
                }

                free(arg_pair);
            }

            res.player.position = player_start_pos;
            res.player.pitch = player_start_pitch;
            res.player.yaw = player_start_yaw;
        }
        else if (string_view_equals_c_str(&parts[0], "object"))
        {
            Model model = {0};

            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (string_view_equals_c_str(&arg_pair[0], "path"))
                {
                    String_View prefix;
                    string_view_create_c_str(&prefix, PREFIX, 0, UINT32_MAX); // Executable is two layers deep

                    String path = string_concat_sv(&prefix, &arg_pair[1]);

                    Mesh mesh = mesh_load_from_obj(path.c_str);
                    model.mesh = (Mesh*)vector_add_r(&meshes, &mesh);

                    string_destroy(&path);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "color"))
                {
                    model.color = color_create_hex((uint32_t)strtoul(arg_pair[1].c_str, NULL, 16)); // base 16 <=> hex
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_x"))
                {
                    model.position.x = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_y"))
                {
                    model.position.y = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_z"))
                {
                    model.position.z = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_x"))
                {
                    model.scale.x = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_y"))
                {
                    model.scale.y = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_z"))
                {
                    model.scale.z = strtof(arg_pair[1].c_str, NULL);
                }

                free(arg_pair);
            }

            vector_add(&models, &model);
        }
        else if (string_view_equals_c_str(&parts[0], "skybox"))
        {
            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (string_view_equals_c_str(&arg_pair[0], "left"))
                {
                    String_View prefix;
                    string_view_create_c_str(&prefix, PREFIX, 0, UINT32_MAX);

                    String path = string_concat_sv(&prefix, &arg_pair[1]);
                    res.skybox.left_tex_path = path.c_str;
                }
                else if (string_view_equals_c_str(&arg_pair[0], "right"))
                {
                    String_View prefix;
                    string_view_create_c_str(&prefix, PREFIX, 0, UINT32_MAX);

                    String path = string_concat_sv(&prefix, &arg_pair[1]);
                    res.skybox.right_tex_path = path.c_str;
                }
                else if (string_view_equals_c_str(&arg_pair[0], "bottom"))
                {
                    String_View prefix;
                    string_view_create_c_str(&prefix, PREFIX, 0, UINT32_MAX);

                    String path = string_concat_sv(&prefix, &arg_pair[1]);
                    res.skybox.bottom_tex_path = path.c_str;
                }
                else if (string_view_equals_c_str(&arg_pair[0], "top"))
                {
                    String_View prefix;
                    string_view_create_c_str(&prefix, PREFIX, 0, UINT32_MAX);

                    String path = string_concat_sv(&prefix, &arg_pair[1]);
                    res.skybox.top_tex_path = path.c_str;
                }
                else if (string_view_equals_c_str(&arg_pair[0], "front"))
                {
                    String_View prefix;
                    string_view_create_c_str(&prefix, PREFIX, 0, UINT32_MAX);

                    String path = string_concat_sv(&prefix, &arg_pair[1]);
                    res.skybox.front_tex_path = path.c_str;
                }
                else if (string_view_equals_c_str(&arg_pair[0], "back"))
                {
                    String_View prefix;
                    string_view_create_c_str(&prefix, PREFIX, 0, UINT32_MAX);

                    String path = string_concat_sv(&prefix, &arg_pair[1]);
                    res.skybox.back_tex_path = path.c_str;
                }
                else if (string_view_equals_c_str(&arg_pair[0], "size"))
                {
                    res.skybox.size = strtof(arg_pair[1].c_str, NULL);
                }

                free(arg_pair);
            }
        }
        else if (string_view_equals_c_str(&parts[0], "light"))
        {
            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (string_view_equals_c_str(&arg_pair[0], "direction_x"))
                {
                    res.light_dir.x = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "direction_y"))
                {
                    res.light_dir.y = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "direction_z"))
                {
                    res.light_dir.z = strtof(arg_pair[1].c_str, NULL);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "color"))
                {
                    res.light_color = color_create_hex((uint32_t)strtoul(arg_pair[1].c_str, NULL, 16)); // base 16 <=> hex
                }

                free(arg_pair);
            }
        }

        free(parts);
    }

    free(lines);
    string_destroy(&source_str);

    res.num_models = models.size;
    res.meshes = (Mesh*)meshes.arr;
    res.models = (Model*)models.arr;
    
    return res;
}

IC_ERROR_CODE level_create(Level *level)
{
    level->_loader.num_resources = level->num_models + 2;
    loader_create(&level->_loader);
    
    for (uint32_t i = 0; i < level->num_models; ++i)
    {
        loader_add_resource(&level->_loader, (Mesh*)level->meshes + i, (Loader_Init_proc)mesh_create, (Loader_Dest_proc)mesh_destroy);
    }

    loader_add_resource(&level->_loader, &level->player, (Loader_Init_proc)player_create, (Loader_Dest_proc)player_destroy);
    loader_add_resource(&level->_loader, &level->skybox, (Loader_Init_proc)skybox_create, (Loader_Dest_proc)skybox_destroy);
    loader_load(&level->_loader);

    // Must delete skybox texture paths (Allocated strings that were never destroyed)
    free((void*)level->skybox.left_tex_path);
    free((void*)level->skybox.right_tex_path);
    free((void*)level->skybox.bottom_tex_path);
    free((void*)level->skybox.top_tex_path);
    free((void*)level->skybox.front_tex_path);
    free((void*)level->skybox.back_tex_path);

    return loader_error(&level->_loader);
}

void level_destroy(const Level *level)
{
    loader_unload((Loader*)&level->_loader);
    loader_destroy(&level->_loader);

    free((void*)level->meshes);
    free((void*)level->models);
}

void level_update(Level *level, Camera *camera, float dt)
{
    level->player.hit_ground = IC_FALSE;

    // Collision
    Collision_Result result;

    typedef Collision_Result (*Collider)(void *capsule, const Model *model, const Player *player, float dt);
    Collider collide;

    switch (level->player.collidable_type)
    {
        case IC_COLLIDABLE_SPHERE: collide = (Collider)collide_sphere_with_static; break;
        case IC_COLLIDABLE_CAPSULE: collide = (Collider)collide_capsule_with_static; break;
        default: return;
    }

    const Model *itr = level->models;

    for (uint32_t i = 0; i < level->num_models; ++i)
    {
        result = collide(level->player.collidable, itr++, &level->player, dt);
        level->player.position = vec3_add(&level->player.position, &result.displacement);
        level->player.velocity = result.res_velocity;
        level->player.hit_ground |= result.hit_ground;
    }

    player_update_camera(&level->player, camera);
}

void level_render(const Level *level, const Window *window, const Camera *camera)
{
    Mat4 view = camera_view_matrix(camera);
    Mat4 projection = mat4_make_project(camera->fov, window_aspect_ratio(window), 0.01f, 1000.0f);

    skybox_shader_bind(level->skybox_shader);
    skybox_shader_set_view_projection(level->skybox_shader, &view, &projection);
    skybox_shader_set_skybox(level->skybox_shader, &level->skybox);
    skybox_render(&level->skybox);

    mesh_shader_bind(level->mesh_shader);
    mesh_shader_set_view(level->mesh_shader, &view);
    mesh_shader_set_projection(level->mesh_shader, &projection);

    const Model *itr = level->models;
    for (uint32_t i = 0; i < level->num_models; ++i)
    {
        Mat4 model_transform = model_transform_matrix(itr);
        mesh_shader_set_transform(level->mesh_shader, &model_transform);
        mesh_shader_set_color(level->mesh_shader, &itr->color);
        model_render(itr);

        ++itr;
    }
}