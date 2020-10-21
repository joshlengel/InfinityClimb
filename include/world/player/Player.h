#pragma once

#include"window/Input.h"
#include"util/math/Vec.h"
#include"world/model/physics/Collidable.h"
#include"Core.h"

enum _Player_Type
{
    IC_PLAYER_SUPER,
    IC_PLAYER_NORMAL
};

typedef enum _Player_Type Player_Type;

struct _Player
{
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;
    IC_BOOL hit_ground;

    float pitch, yaw;

    Player_Type type;

    void *collidable;
    Collidable collidable_type;
    Vec3 collidable_offset;

    Vec3 cam_offset;
};

typedef struct _Player Player;

typedef struct _Camera Camera;

IC_ERROR_CODE player_create(Player *dest);
void player_destroy(const Player *player);
void player_move_forward(Player *player, float speed);
void player_move_right(Player *player, float speed);
void player_move_up(Player *player, float speed);
void player_update_camera(Player *player, Camera *camera);

Player player_load_from_file(const char *path);

struct _Player_Controller
{
    IC_KEY forward_key;
    IC_KEY backward_key;
    IC_KEY left_key;
    IC_KEY right_key;

    IC_KEY up_key;
    IC_KEY down_key;

    float xz_speed;
    float y_speed;

    float mouse_sensitivity;

    Player *player;
};

typedef struct _Player_Controller Player_Controller;

typedef struct _Timer Timer;

void player_controller_update(const Player_Controller *controller, const Input *input, float dt);