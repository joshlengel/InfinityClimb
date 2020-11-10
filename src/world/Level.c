#include"world/Level.h"

#include"world/Camera.h"
#include"window/Window.h"

#include"world/model/mesh/Mesh.h"
#include"world/model/mesh/Mesh_Shader.h"
#include"world/model/skybox/Skybox.h"
#include"world/model/skybox/Skybox_Shader.h"
#include"world/model/Model.h"
#include"world/target/Target.h"
#include"world/target/Raycast.h"

#include"util/Utils.h"
#include"util/Vector.h"
#include"Log.h"

#include<stdlib.h>
#include<math.h>

#ifdef IC_DEBUG
const char *PLAYER_OPTION_NAMES[6] =
{
    "path", "start_x", "start_y", "start_z", "start_pitch", "start_yaw"
};

const char *OBJECT_OPTION_NAMES[8] =
{
    "path", "color", "position_x", "position_y", "position_z", "scale_x", "scale_y", "scale_z"
};

const char *TARGET_OPTION_NAMES[8] =
{
    "path", "color", "position_x", "position_y", "position_z", "scale_x", "scale_y", "scale_z"
};

const char *SKYBOX_OPTION_NAMES[7] =
{
    "left", "right", "bottom", "top", "front", "back", "size"
};

const char *LIGHT_OPTION_NAMES[4] =
{
    "direction_x", "direction_y", "direction_z", "color"
};

const char *LEVEL_IDENTIFIER_NAMES[3] =
{
    "player", "skybox", "light"
};

void __level_check_required_options_impl(IC_OPTIONS_32 options, uint8_t num_options, const char *part_name, const char **option_names, uint32_t line, const char *path, IC_ERROR_CODE *error_code)
{
    for (uint8_t i = 0; i < num_options; ++i)
    {
        if (!IC_DEBUG_OPTION_IS_SET(options, i))
        {
            log_trace("Error on line %u at '%s': Option '%s' of %s must be set", line + 1, path, option_names[i], part_name);
            if (error_code) *error_code = IC_LEVEL_INCOMPLETE_ERROR;
        }
    }
}

#endif // IC_DEBUG

#ifdef IC_DEBUG
    #define __check_required_options(options, num_options, part_name, option_names, line, path, error_code) __level_check_required_options_impl(options, num_options, part_name, option_names, line, path, error_code)
#else
    #define __check_required_options(options, num_options, part_name, option_names, line, path, error_code)
#endif // IC_DEBUG

Level level_load_from_file(const char *path, IC_ERROR_CODE *error_code)
{
    if (error_code) *error_code = IC_NO_ERROR;

    Level res = {0};
    Vec3 player_start_pos;
    float player_start_pitch;
    float player_start_yaw;

    IC_ERROR_CODE ec;
    String source_str = read_source(path, &ec);
    if (ec != IC_NO_ERROR)
    {
        if (error_code) *error_code = ec;
        return res;
    }

    String_View source;
    string_view_create_s(&source, &source_str, 0, UINT32_MAX);

    Vector meshes = {.elem_size=sizeof(Mesh), .init_capacity=5};
    Vector models = {.elem_size=sizeof(Model), .init_capacity=5};
    Vector targets = {.elem_size=sizeof(Target), .init_capacity=5};
    ec = vector_create(&meshes);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&source_str);
        if (error_code) *error_code = ec;
        return res;
    }

    ec = vector_create(&models);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&source_str);
        vector_destroy(&meshes);
        if (error_code) *error_code = ec;
        return res;
    }

    ec = vector_create(&targets);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&source_str);
        vector_destroy(&meshes);
        vector_destroy(&models);
        if (error_code) *error_code = ec;
        return res;
    }

    uint32_t num_lines;
    String_View *lines = string_view_split(&source, '\n', &num_lines);

    IC_DEBUG_OPTIONS_32(level_options);

    for (uint32_t l = 0; l < num_lines; ++l)
    {
        String_View line = lines[l];
        line = string_view_trim(&line);

        uint32_t num_parts;
        String_View *parts = string_view_split(&line, ' ', &num_parts);

        if (num_parts == 0) // Not really an error
        {
            free(parts);
            log_trace("Warning on line %u at '%s': Empty line with whitespace found. This is not critical, but leads to inefficiencies", l + 1, path);
            if (error_code) *error_code = IC_LEVEL_WHITESPACE_INEFFICIENCY;
            continue;
        }

        if (string_view_equals_c_str(&parts[0], "player"))
        {
            IC_DEBUG_OPTIONS_32(options)

            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (num_arg_pair == 0) // Incorrect syntax
                {
                    log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", l + 1, path, parts[i].length, parts[i].c_str);
                    free(arg_pair);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                    continue;
                }

                if (string_view_equals_c_str(&arg_pair[0], "path"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    res.player = player_load_from_file(path.c_str, error_code);
                    if (*error_code != IC_NO_ERROR)
                    {
                        free(arg_pair);
                        continue;
                    }

                    string_destroy(&path);
                    IC_DEBUG_OPTION_SET(options, 0);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_x"))
                {
                    player_start_pos.x = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 1);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_y"))
                {
                    player_start_pos.y = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 2);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_z"))
                {
                    player_start_pos.z = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 3);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_pitch"))
                {
                    player_start_pitch = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 4);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "start_yaw"))
                {
                    player_start_yaw = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 5);
                }
                else
                {
                    log_trace("Warning on line %u at '%s': Unrecognized option '%.*s' for player", l + 1, path, arg_pair[0].length, arg_pair[0].c_str);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                }

                free(arg_pair);
            }

            __check_required_options(options, 6, "player", PLAYER_OPTION_NAMES, l + 1, path, error_code);

            res.player.position = player_start_pos;
            res.player.pitch = player_start_pitch;
            res.player.yaw = player_start_yaw;

            IC_DEBUG_OPTION_SET(level_options, 0);
        }
        else if (string_view_equals_c_str(&parts[0], "object"))
        {
            Model model = {0};

            IC_DEBUG_OPTIONS_32(options)

            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (num_arg_pair == 0) // Incorrect syntax
                {
                    log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", l + 1, path, parts[i].length, parts[i].c_str);
                    free(arg_pair);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                    continue;
                }

                if (string_view_equals_c_str(&arg_pair[0], "path"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    Mesh mesh = mesh_load_from_obj(path.c_str, &ec);
                    if (ec != IC_NO_ERROR)
                    {
                        free(arg_pair);
                        string_destroy(&path);
                        continue;
                    }

                    model.mesh = (Mesh*)vector_add_r(&meshes, &mesh);

                    string_destroy(&path);
                    IC_DEBUG_OPTION_SET(options, 0);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "color"))
                {
                    model.color = color_create_hex((uint32_t)strtoul(arg_pair[1].c_str, NULL, 16)); // base 16 <=> hex
                    IC_DEBUG_OPTION_SET(options, 1);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_x"))
                {
                    model.position.x = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 2);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_y"))
                {
                    model.position.y = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 3);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_z"))
                {
                    model.position.z = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 4);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_x"))
                {
                    model.scale.x = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 5);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_y"))
                {
                    model.scale.y = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 6);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_z"))
                {
                    model.scale.z = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 7);
                }
                else
                {
                    log_trace("Warning on line %u at '%s': Unrecognized option '%.*s' for object", l + 1, path, arg_pair[0].length, arg_pair[0].c_str);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                }

                free(arg_pair);
            }

            __check_required_options(options, 8, "object", OBJECT_OPTION_NAMES, l, path, error_code);

            vector_add(&models, &model);
        }
        else if (string_view_equals_c_str(&parts[0], "target"))
        {
            Target target;

            IC_DEBUG_OPTIONS_32(options)

            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (num_arg_pair == 0) // Incorrect syntax
                {
                    log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", l + 1, path, parts[i].length, parts[i].c_str);
                    free(arg_pair);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                    continue;
                }

                if (string_view_equals_c_str(&arg_pair[0], "path"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    target = target_load(path.c_str, &ec);
                    if (ec != IC_NO_ERROR)
                    {
                        free(arg_pair);
                        string_destroy(&path);
                        continue;
                    }

                    string_destroy(&path);
                    IC_DEBUG_OPTION_SET(options, 0);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "color"))
                {
                    target.model.color = color_create_hex((uint32_t)strtoul(arg_pair[1].c_str, NULL, 16)); // base 16 <=> hex
                    IC_DEBUG_OPTION_SET(options, 1);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_x"))
                {
                    target.model.position.x = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 2);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_y"))
                {
                    target.model.position.y = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 3);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "position_z"))
                {
                    target.model.position.z = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 4);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_x"))
                {
                    target.model.scale.x = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 5);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_y"))
                {
                    target.model.scale.y = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 6);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scale_z"))
                {
                    target.model.scale.z = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 7);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "yaw"))
                {
                    target.model.rotation_axis.x = 0.0f;
                    target.model.rotation_axis.y = 1.0f;
                    target.model.rotation_axis.z = 0.0f;
                    target.model.rotation = strtof(arg_pair[1].c_str, NULL);
                    // Optional
                }
                else
                {
                    log_trace("Warning on line %u at '%s': Unrecognized option '%.*s' for target", l + 1, path, arg_pair[0].length, arg_pair[0].c_str);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                }

                free(arg_pair);
            }

            __check_required_options(options, 8, "target", TARGET_OPTION_NAMES, l, path, error_code);

            vector_add(&targets, &target);
        }
        else if (string_view_equals_c_str(&parts[0], "skybox"))
        {
            IC_DEBUG_OPTIONS_32(options)

            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (num_arg_pair == 0) // Incorrect syntax
                {
                    log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", l + 1, path, parts[i].length, parts[i].c_str);
                    free(arg_pair);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                    continue;
                }

                if (string_view_equals_c_str(&arg_pair[0], "left"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    res.skybox.left_tex_path = path.c_str;
                    IC_DEBUG_OPTION_SET(options, 0);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "right"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    res.skybox.right_tex_path = path.c_str;
                    IC_DEBUG_OPTION_SET(options, 1);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "bottom"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    res.skybox.bottom_tex_path = path.c_str;
                    IC_DEBUG_OPTION_SET(options, 2);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "top"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    res.skybox.top_tex_path = path.c_str;
                    IC_DEBUG_OPTION_SET(options, 3);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "front"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    res.skybox.front_tex_path = path.c_str;
                    IC_DEBUG_OPTION_SET(options, 4);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "back"))
                {
                    String path;
                    string_create_sv(&path, &arg_pair[1]);

                    res.skybox.back_tex_path = path.c_str;
                    IC_DEBUG_OPTION_SET(options, 5);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "size"))
                {
                    res.skybox.size = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 6);
                }
                else
                {
                    log_trace("Warning on line %u at '%s': Unrecognized option '%.*s' for skybox", l + 1, path, arg_pair[0].length, arg_pair[0].c_str);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                }

                free(arg_pair);
            }

            __check_required_options(options, 7, "skybox", SKYBOX_OPTION_NAMES, l, path, error_code);

            IC_DEBUG_OPTION_SET(level_options, 1);
        }
        else if (string_view_equals_c_str(&parts[0], "light"))
        {
            IC_DEBUG_OPTIONS_32(options)

            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (num_arg_pair == 0) // Incorrect syntax
                {
                    log_trace("Warning on line %u at '%s': '%.*s' is incorrect syntax", l + 1, path, parts[i].length, parts[i].c_str);
                    free(arg_pair);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                    continue;
                }

                if (string_view_equals_c_str(&arg_pair[0], "direction_x"))
                {
                    res.light_dir.x = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 0);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "direction_y"))
                {
                    res.light_dir.y = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 1);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "direction_z"))
                {
                    res.light_dir.z = strtof(arg_pair[1].c_str, NULL);
                    IC_DEBUG_OPTION_SET(options, 2);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "color"))
                {
                    res.light_color = color_create_hex((uint32_t)strtoul(arg_pair[1].c_str, NULL, 16)); // base 16 <=> hex
                    IC_DEBUG_OPTION_SET(options, 3);
                }
                else
                {
                    log_trace("Warning on line %u at '%s': Unrecognized option '%.*s' for light", l + 1, path, arg_pair[0].length, arg_pair[0].c_str);
                    if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
                }

                free(arg_pair);
            }

            __check_required_options(options, 4, "light", LIGHT_OPTION_NAMES, l, path, error_code);

            IC_DEBUG_OPTION_SET(level_options, 2);
        }
        else
        {
            log_trace("Warning on line %u at '%s': Unrecognized identifier '%.*s'", l + 1, path, parts[0].length, parts[0].c_str);
            if (error_code) *error_code = IC_LEVEL_SYNTAX_ERROR;
        }

        free(parts);
    }

    free(lines);
    string_destroy(&source_str);

    // Check to make sure all required identifiers are present
#ifdef IC_DEBUG
    for (uint8_t i = 0; i < 3; ++i)
    {
        if (!IC_DEBUG_OPTION_IS_SET(level_options, i))
        {
            log_trace("Error in level file at '%s': Missing options of identifier '%s'", path, LEVEL_IDENTIFIER_NAMES[i]);
            if (error_code) *error_code = IC_LEVEL_INCOMPLETE_ERROR;
        }
    }
#endif // IC_DEBUG

    res.num_models = models.size;
    res.meshes = (Mesh*)meshes.arr;
    res.models = (Model*)models.arr;

    res.num_targets = targets.size;
    res.targets = (Target*)targets.arr;

    return res;
}

IC_ERROR_CODE level_create(Level *level)
{
    level->_loader.num_resources = level->num_models + level->num_targets + 2;
    loader_create(&level->_loader);
    
    Mesh *mesh_itr = level->meshes;
    for (uint32_t i = 0; i < level->num_models; ++i)
    {
        loader_add_resource(&level->_loader, mesh_itr++, (Loader_Init_proc)mesh_create, (Loader_Dest_proc)mesh_destroy);
    }

    Target *target_itr = level->targets;
    for (uint32_t i = 0; i < level->num_targets; ++i)
    {
        loader_add_resource(&level->_loader, target_itr++, (Loader_Init_proc)target_create, (Loader_Dest_proc)target_destroy);
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
    free((void*)level->targets);
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
        default: log_assert(IC_FALSE, "Unknown collidable type '%u'", (uint32_t)level->player.collidable_type) return;
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

    const Model *model_itr = level->models;
    for (uint32_t i = 0; i < level->num_models; ++i)
    {
        Mat4 model_transform = model_transform_matrix(model_itr);
        mesh_shader_set_transform(level->mesh_shader, &model_transform);
        mesh_shader_set_color(level->mesh_shader, &model_itr->color);
        model_render(model_itr);

        ++model_itr;
    }

    const Target *target_itr = level->targets;
    for (uint32_t i = 0; i < level->num_targets; ++i)
    {
        if (target_itr->active) 
        {
            Mat4 model_transform = model_transform_matrix(&target_itr->model);
            mesh_shader_set_transform(level->mesh_shader, &model_transform);
            mesh_shader_set_color(level->mesh_shader, &target_itr->model.color);
            model_render(&target_itr->model);
        }

        ++target_itr;
    }

    Mat4 player_transform = player_transform_matrix(&level->player);
    Color player_color = color_create_hex(0x999999FF);
    mesh_shader_set_transform(level->mesh_shader, &player_transform);
    mesh_shader_set_color(level->mesh_shader, &player_color);
    mesh_render(&level->player.mesh);    
}

void level_shoot(Level *level)
{
    // Find ray
    Vec3 ray_origin = vec3_add(&level->player.position, &level->player.ray_cast_offset);
    float cos_pitch = cosf(level->player.pitch);
    Vec3 ray_direction =
    {
        sinf(level->player.yaw) * cos_pitch,
        sinf(level->player.pitch),
        cosf(level->player.yaw) * cos_pitch
    };

    RaycastResult res = { .collision=IC_FALSE, .distance=INFINITY };
    Target *hit = NULL;

    Target *itr = level->targets;
    for (uint32_t i = 0; i < level->num_targets; ++i)
    {
        RaycastResult c_res = raycast_check_target(&ray_origin, &ray_direction, 10.0f, itr);
        if (c_res.distance < res.distance)
        {
            res = c_res;
            hit = itr;
        }

        ++itr;
    }

    if (res.collision)
    {
        hit->active = IC_FALSE;
    }
}