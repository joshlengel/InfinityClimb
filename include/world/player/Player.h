#pragma once

#include"world/player/Crosshairs.h"
#include"world/model/physics/Collidable.h"
#include"world/model/mesh/Mesh.h"

#include"window/Input.h"
#include"util/math/Vec.h"
#include"util/math/Mat.h"
#include"Core.h"

enum _Player_Type
{
    IC_PLAYER_SUPER,
    IC_PLAYER_NORMAL
};

enum _Player_Perspective
{
    IC_PLAYER_FIRST_PERSON,
    IC_PLAYER_THIRD_PERSON
};

typedef enum _Player_Type Player_Type;
typedef enum _Player_Perspective Player_Perspective;

struct _Player
{
    // Player state
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;
    IC_BOOL hit_ground;

    float pitch, yaw;

    Player_Type type;
    Player_Perspective perspective;

    // Physics
    void *collidable;
    Collidable collidable_type;
    Vec3 collidable_offset;

    // Renderable
    Mesh mesh;
    Vec3 mesh_offset;
    Crosshairs crosshairs;

    // Camera
    Vec3 cam_offset;
    float cam_dist; // Only for third person
    float cam_pitch, cam_yaw;

    // Raycasting
    Vec3 ray_cast_offset; // Used for shooting
};

typedef struct _Player Player;

typedef struct _Camera Camera;

IC_ERROR_CODE player_create(Player *dest);
void player_destroy(const Player *player);
void player_move_forward(Player *player, float speed);
void player_move_right(Player *player, float speed);
void player_move_up(Player *player, float speed);
void player_update_camera(Player *player, Camera *camera);

Player player_load_from_file(const char *path, IC_ERROR_CODE *ec);

Mat4 player_transform_matrix(const Player *player);

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

typedef struct _Input Input;

void player_controller_update(const Player_Controller *controller, const Input *input, float dt);