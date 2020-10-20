#include"world/model/physics/Collidable.h"
#include"world/model/mesh/Mesh.h"
#include"world/model/Model.h"
#include"world/player/Player.h"

#include<math.h>

Vec3 closest_to_line(const Vec3 *a, const Vec3 *b, const Vec3 *point)
{
    Vec3 ab = vec3_sub(b, a);
    Vec3 ap = vec3_sub(point, a);
    float t = vec3_dot(&ap, &ab) / vec3_length_sqr(&ab);

    if (t < 0) return *a;
    else if (t > 1) return *b;
    
    Vec3 d_ab = vec3_scale(&ab, t);
    return vec3_add(a, &d_ab);
}

Collision_Result collide_sphere_with_static(const Sphere *sphere, const Model *model, const Player *player, float dt)
{
    /*Collision_Result result;
    result.time_simulated = dt;
    result.res_velocity = player->velocity;
    result.res_temp_velocity = player->accum_velocity;*/
    Collision_Result result;
    result.displacement.x = 0.0f;
    result.displacement.y = 0.0f;
    result.displacement.z = 0.0f;
    result.res_velocity = player->velocity;

    uint32_t num_indices = model->mesh->num_indices / 3;
    const uint32_t *index_itr = model->mesh->indices;

    for (uint32_t i = 0; i < num_indices; ++i)
    {
        const float *v0_itr = model->mesh->vertices + *(index_itr++) * 3;
        const float *v1_itr = model->mesh->vertices + *(index_itr++) * 3;
        const float *v2_itr = model->mesh->vertices + *(index_itr++) * 3;

        Vec3 p0 = { *(v0_itr++), *(v0_itr++), *(v0_itr) };
        Vec3 p1 = { *(v1_itr++), *(v1_itr++), *(v1_itr) };
        Vec3 p2 = { *(v2_itr++), *(v2_itr++), *(v2_itr) };

        Vec3 edge1 = vec3_sub(&p1, &p0);
        Vec3 edge2 = vec3_sub(&p2, &p0);
        Vec3 N = vec3_cross(&edge1, &edge2);
        N = vec3_normalize(&N);

        Vec3 center = player->position;

        Vec3 p0_center = vec3_sub(&center, &p0);
        float plane_dist = vec3_dot(&p0_center, &N);

        if (plane_dist < -sphere->radius || plane_dist > sphere->radius) continue;

        Vec3 scaled_normal = vec3_scale(&N, plane_dist);
        Vec3 projected_center = vec3_sub(&center, &scaled_normal);

        Vec3 p0_projected_center = vec3_sub(&projected_center, &p0);
        Vec3 p1_projected_center = vec3_sub(&projected_center, &p1);
        Vec3 p2_projected_center = vec3_sub(&projected_center, &p2);

        Vec3 p0_p1 = vec3_sub(&p1, &p0);
        Vec3 p1_p2 = vec3_sub(&p2, &p1);
        Vec3 p2_p0 = vec3_sub(&p0, &p2);

        Vec3 c0 = vec3_cross(&p0_projected_center, &p0_p1);
        Vec3 c1 = vec3_cross(&p1_projected_center, &p1_p2);
        Vec3 c2 = vec3_cross(&p2_projected_center, &p2_p0);
        IC_BOOL projected_inside = vec3_dot(&c0, &N) <= 0 && vec3_dot(&c1, &N) <= 0 && vec3_dot(&c2, &N) <= 0;

        float rad_sqr = sphere->radius * sphere->radius;
        IC_BOOL intersects = IC_FALSE;

        Vec3 projected_edge0 = closest_to_line(&p0, &p1, &center);
        Vec3 v0 = vec3_sub(&center, &projected_edge0);
        float dist_sqr0 = vec3_length_sqr(&v0);
        intersects |= dist_sqr0 < rad_sqr;

        Vec3 projected_edge1 = closest_to_line(&p1, &p2, &center);
        Vec3 v1 = vec3_sub(&center, &projected_edge1);
        float dist_sqr1 = vec3_length_sqr(&v1);
        intersects |= dist_sqr1 < rad_sqr;

        Vec3 projected_edge2 = closest_to_line(&p2, &p0, &center);
        Vec3 v2 = vec3_sub(&center, &projected_edge2);
        float dist_sqr2 = vec3_length_sqr(&v2);
        intersects |= dist_sqr2 < rad_sqr;

        if (projected_inside || intersects)
        {
            Vec3 intersect = projected_center;
            Vec3 intersect_d;

            if (projected_inside)
            {
                intersect_d = vec3_sub(&center, &projected_center);
            }
            else
            {
                Vec3 d;
                float min_dist_sqr = INFINITY;

                d = vec3_sub(&center, &projected_edge0);
                dist_sqr0 = vec3_length_sqr(&d);
                if (dist_sqr0 < min_dist_sqr)
                {
                    min_dist_sqr = dist_sqr0;
                    intersect = projected_edge0;
                    intersect_d = d;
                }

                d = vec3_sub(&center, &projected_edge1);
                dist_sqr1 = vec3_length_sqr(&d);
                if (dist_sqr1 < min_dist_sqr)
                {
                    min_dist_sqr = dist_sqr1;
                    intersect = projected_edge1;
                    intersect_d = d;
                }

                d = vec3_sub(&center, &projected_edge2);
                dist_sqr2 = vec3_length_sqr(&d);
                if (dist_sqr2 < min_dist_sqr)
                {
                    min_dist_sqr = dist_sqr2;
                    intersect = projected_edge2;
                    intersect_d = d;
                }
            }

            float length = vec3_length(&intersect_d);
            Vec3 collision_normal = vec3_scale(&intersect_d, 1.0f / length); // normalize
            float collision_depth = sphere->radius - length;
            Vec3 response = vec3_scale(&collision_normal, collision_depth);
            result.displacement = vec3_add(&result.displacement, &response);

            float speed = vec3_length(&player->velocity);
            Vec3 vel_norm = vec3_scale(&player->velocity, 1.0f / speed);
            Vec3 absorbed = vec3_scale(&collision_normal, vec3_dot(&vel_norm, &collision_normal));
            Vec3 resulting = vec3_sub(&vel_norm, &absorbed);
            result.res_velocity = vec3_scale(&resulting, speed);
        }
    }

    return result;
}