#include"world/Physics.h"

float EPSILON = 0.01f; // Maximum error (Float imprecision causes objects to fall through each other)

Collision_Result physics_collide(const AABB *obj1, const Vec3 *vel1, const Vec3 *accum_vel1, const AABB *obj2, float dt)
{
    Vec3 sum_vel = vec3_add(vel1, accum_vel1);

    Collision_Result result;
    result.obj1_displacement = vec3_scale(&sum_vel, dt);
    result.obj1_vel = *vel1;
    result.obj1_accum_vel = *accum_vel1;
    result.time_simulated = dt;

    float left1   = obj1->center.x - obj1->extent.x;
    float right1  = obj1->center.x + obj1->extent.x;
    float bottom1 = obj1->center.y - obj1->extent.y;
    float top1    = obj1->center.y + obj1->extent.y;
    float front1  = obj1->center.z - obj1->extent.z;
    float back1   = obj1->center.z + obj1->extent.z;

    float left2   = obj2->center.x - obj2->extent.x;
    float right2  = obj2->center.x + obj2->extent.x;
    float bottom2 = obj2->center.y - obj2->extent.y;
    float top2    = obj2->center.y + obj2->extent.y;
    float front2  = obj2->center.z - obj2->extent.z;
    float back2   = obj2->center.z + obj2->extent.z;

    if (right1 <= left2 + EPSILON && sum_vel.x > 0)
    {
        // right1 + vx * t = left2 => t = (left2 - right1) / vx
        float t = (left2 - right1) / sum_vel.x;

        if (t <= dt) // collision might have occured
        {
            Vec3 d_pos = vec3_scale(&sum_vel, t);
            float n_bottom1 = bottom1 + d_pos.y;
            float n_top1    = top1    + d_pos.y;
            float n_front1  = front1  + d_pos.z;
            float n_back1   = back1   + d_pos.z;

            if (n_bottom1 < top2 && n_top1 > bottom2 && n_front1 < back2 && n_back1 > front2) // collision actually occured
            {
                result.time_simulated = t;
                result.obj1_displacement = d_pos;
                result.obj1_vel.x = 0.0;
                result.obj1_accum_vel.x = 0.0;
                return result;
            }
        }
    }
    else if (left1 >= right2 - EPSILON && sum_vel.x < 0)
    {
        // left1 + vx * t = right2 => t = (right2 - left1) / vx
        float t = (right2 - left1) / sum_vel.x;

        if (t <= dt) // collision might have occured
        {
            Vec3 d_pos = vec3_scale(&sum_vel, t);
            float n_bottom1 = bottom1 + d_pos.y;
            float n_top1    = top1    + d_pos.y;
            float n_front1  = front1  + d_pos.z;
            float n_back1   = back1   + d_pos.z;

            if (n_bottom1 < top2 && n_top1 > bottom2 && n_front1 < back2 && n_back1 > front2) // collision actually occured
            {
                result.time_simulated = t;
                result.obj1_displacement = d_pos;
                result.obj1_vel.x = 0.0;
                result.obj1_accum_vel.x = 0.0;
                return result;
            }
        }
    }

    if (top1 <= bottom2 + EPSILON && sum_vel.y > 0)
    {
        // top1 + vy * t = bottom2 => t = (bottom2 - top1) / vy
        float t = (bottom2 - top1) / sum_vel.y;

        if (t <= dt) // collision might have occured
        {
            Vec3 d_pos = vec3_scale(&sum_vel, t);
            float n_left1  = left1  + d_pos.x;
            float n_right1 = right1 + d_pos.x;
            float n_front1 = front1 + d_pos.z;
            float n_back1  = back1  + d_pos.z;

            if (n_left1 < right2 && n_right1 > left2 && n_front1 < back2 && n_back1 > front2) // collision actually occured
            {
                result.time_simulated = t;
                result.obj1_displacement = d_pos;
                result.obj1_vel.y = 0.0;
                result.obj1_accum_vel.y = 0.0;
                return result;
            }
        }
    }
    else if (bottom1 >= top2 - EPSILON && sum_vel.y < 0)
    {
        // bottom1 + vy * t = top2 => t = (top2 - bottom1) / vy
        float t = (top2 - bottom1) / sum_vel.y;

        if (t <= dt) // collision might have occured
        {
            Vec3 d_pos = vec3_scale(&sum_vel, t);
            float n_left1  = left1  + d_pos.x;
            float n_right1 = right1 + d_pos.x;
            float n_front1 = front1 + d_pos.z;
            float n_back1  = back1  + d_pos.z;

            if (n_left1 < right2 && n_right1 > left2 && n_front1 < back2 && n_back1 > front2) // collision actually occured
            {
                result.time_simulated = t;
                result.obj1_displacement = d_pos;
                result.obj1_vel.y = 0.0;
                result.obj1_accum_vel.y = 0.0;
                return result;
            }
        }
    }

    if (back1 <= front2 + EPSILON && sum_vel.z > 0)
    {
        // back1 + vz * t = front2 => t = (front2 - back1) / vz
        float t = (front2 - back1) / sum_vel.z;

        if (t <= dt) // collision might have occured
        {
            Vec3 d_pos = vec3_scale(&sum_vel, t);
            float n_left1   = left1   + d_pos.x;
            float n_right1  = right1  + d_pos.x;
            float n_bottom1 = bottom1 + d_pos.y;
            float n_top1    = top1    + d_pos.y;

            if (n_left1 < right2 && n_right1 > left2 && n_bottom1 < top2 && n_top1 > bottom2) // collision actually occured
            {
                result.time_simulated = t;
                result.obj1_displacement = d_pos;
                result.obj1_vel.z = 0.0;
                result.obj1_accum_vel.z = 0.0;
                return result;
            }
        }
    }
    else if (front1 >= back2 - EPSILON && sum_vel.z < 0)
    {
        // front1 + vz * t = back2 => t = (back2 - front1) / vz
        float t = (back2 - front1) / sum_vel.z;

        if (t <= dt) // collision might have occured
        {
            Vec3 d_pos = vec3_scale(&sum_vel, t);
            float n_left1   = left1   + d_pos.x;
            float n_right1  = right1  + d_pos.x;
            float n_bottom1 = bottom1 + d_pos.y;
            float n_top1    = top1    + d_pos.y;

            if (n_left1 < right2 && n_right1 > left2 && n_bottom1 < top2 && n_top1 > bottom2) // collision actually occured
            {
                result.time_simulated = t;
                result.obj1_displacement = d_pos;
                result.obj1_vel.z = 0.0;
                result.obj1_accum_vel.z = 0.0;
                return result;
            }
        }
    }

    return result;
}