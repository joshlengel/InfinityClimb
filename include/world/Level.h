#pragma once

#include"world/player/Player.h"
#include"world/model/skybox/Skybox.h"

#include"util/math/Vec.h"
#include"util/Color.h"
#include"util/Utils.h"
#include"Core.h"

#include<stdint.h>

typedef struct _Mesh Mesh;
typedef struct _Model Model;

typedef struct _Mesh_Shader Mesh_Shader;
typedef struct _Skybox_Shader Skybox_Shader;

struct _Level
{
    uint32_t num_models;
    const Mesh *meshes;
    const Model *models;

    Skybox skybox;

    Player player;

    Vec3 light_dir;
    Color light_color;

    Mesh_Shader *mesh_shader;
    Skybox_Shader *skybox_shader;

    Loader _loader;
};

typedef struct _Level Level;
typedef struct _Window Window;
typedef struct _Camera Camera;

IC_ERROR_CODE level_create(Level *level);
void level_destroy(const Level *level);

void level_update(Level *level, Camera *camera, float dt);
void level_render(const Level *level, const Window *window, const Camera *camera);

Level level_load_from_file(const char *path);