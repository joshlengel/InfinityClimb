#pragma once

#include"Core.h"
#include"math/Vec.h"
#include<stdint.h>

typedef struct _Rect Rect;
typedef struct _Rect_Shader Rect_Shader;
typedef struct _Window Window;
typedef struct _Camera Camera;
typedef struct _Player Player;

typedef struct _Level_Data Level_Data;

struct _Level
{
    Vec3 start_position;
    float start_pitch, start_yaw;
    uint32_t num_rects;

    Window *window;
    Rect_Shader *rect_shader;
    Camera *camera;
    Player *player;

    Vec3 light_dir;
    
    Level_Data *data;
};

typedef struct _Level Level;

IC_ERROR_CODE level_create(Level *dest);
IC_ERROR_CODE level_create_from_rects(Level *dest);
void level_destroy(const Level *level);

IC_ERROR_CODE level_load(const Level *level);
void level_unload(const Level *level);

void level_add_rect(const Level *level, Rect rect);
void level_update(const Level *level, float dt);
void level_render(const Level *level);