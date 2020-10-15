#include"world/Physics.h"

Collision_Result physics_collide(const AABB *obj1, const Vec3 *vel1, const AABB *obj2, const Vec3 *vel2)
{
    Vec3 zero = { 0.0f, 0.0f, 0.0f };

    Collision_Result result;
    result.obj1_displacement = zero;
    result.obj2_displacement = zero;
    result.obj1_vel = *vel1;
    result.obj2_vel = *vel2;

    if (
        obj2->center.x - obj2->extent.x < obj1->center.x + obj1->extent.x
        && obj2->center.x + obj2->extent.x > obj1->center.x - obj1->extent.x
        && obj2->center.y - obj2->extent.y < obj1->center.y + obj1->extent.y
        && obj2->center.y + obj2->extent.y > obj1->center.y - obj1->extent.y
        && obj2->center.z - obj2->extent.z < obj1->center.z + obj1->extent.z
        && obj2->center.z + obj2->extent.z > obj1->center.z - obj1->extent.z)
    {
        float disp_x = obj1->center.x < obj2->center.x?
            (obj1->center.x + obj1->extent.x) - (obj2->center.x - obj2->extent.x)
            : (obj2->center.x + obj2->extent.x) - (obj1->center.x - obj1->extent.x);
        
        float disp_y = obj1->center.y < obj2->center.y?
            (obj1->center.y + obj1->extent.y) - (obj2->center.y - obj2->extent.y)
            : (obj2->center.y + obj2->extent.y) - (obj1->center.y - obj1->extent.y);
        
        float disp_z = obj1->center.z < obj2->center.z?
            (obj1->center.z + obj1->extent.z) - (obj2->center.z - obj2->extent.z)
            : (obj2->center.z + obj2->extent.z) - (obj1->center.z - obj1->extent.z);
        
        Vec3 disp = { 0.0f, 0.0f, 0.0f };
        
        if (disp_x < disp_y)
        {
            if (disp_x < disp_z)
            {
                disp.x = obj1->center.x < obj2->center.x? -disp_x : disp_x;
                result.obj1_vel.x = 0.0f;
                result.obj2_vel.x = 0.0f;
            }
            else
            {
                disp.z = obj1->center.z < obj2->center.z? -disp_z : disp_z;
                result.obj1_vel.z = 0.0f;
                result.obj2_vel.z = 0.0f; 
            }
        }
        else
        {
            if (disp_y < disp_z)
            {
                disp.y = obj1->center.y < obj2->center.y? -disp_y : disp_y;
                result.obj1_vel.y = 0.0f;
                result.obj2_vel.y = 0.0f;
            }
            else
            {
                disp.z = obj1->center.z < obj2->center.z? -disp_z : disp_z;
                result.obj1_vel.z = 0.0f;
                result.obj2_vel.z = 0.0f;
            }
        }

        disp = vec3_scale(&disp, 0.5f);

        result.obj1_displacement = disp;
        result.obj2_displacement = vec3_negate(&disp);
    }

    return result;
}