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

Collision_Result __collide_sphere_with_triangle_impl(const Sphere *sphere, Vec3 center, const Player *player, Vec3 p0, Vec3 p1, Vec3 p2, float rad_sqr, Vec3 N)
{
    Collision_Result result;
    result.displacement.x = 0.0f;
    result.displacement.y = 0.0f;
    result.displacement.z = 0.0f;
    result.collision_depth = 0.0f;

    Vec3 p0_center = vec3_sub(&center, &p0);
    float plane_dist = vec3_dot(&p0_center, &N);

    if (plane_dist < -sphere->radius || plane_dist > sphere->radius) return result;

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
        Vec3 response = vec3_scale(&collision_normal, collision_depth + 0.0001f);
        result.displacement = response;
        result.collision_depth = collision_depth;

        float speed = vec3_length(&player->velocity);
        Vec3 vel_norm = vec3_scale(&player->velocity, 1.0f / speed);
        Vec3 response_norm = vec3_scale(&collision_normal, vec3_dot(&vel_norm, &collision_normal));
        Vec3 response_vel = vec3_scale(&response_norm, speed);
        result.res_velocity = vec3_sub(&player->velocity, &response_vel);
    }

    return result;
}

Collision_Result collide_sphere_with_static(const Sphere *sphere, const Model *model, const Player *player, float dt)
{
    Collision_Result result;
    result.displacement.x = 0.0f;
    result.displacement.y = 0.0f;
    result.displacement.z = 0.0f;
    result.res_velocity = player->velocity;
    result.collision_depth = 0.0f;
    result.hit_ground = IC_FALSE;

    Mat4 transform = model_transform_matrix(model);
    Vec3 position = vec3_add(&player->position, &player->collidable_offset);

    float rad_sqr = sphere->radius * sphere->radius;

    uint32_t num_indices = model->mesh->num_indices / 3;
    const uint32_t *index_itr = model->mesh->indices;

    for (uint32_t i = 0; i < num_indices; ++i)
    {
        const float *v0_itr = model->mesh->vertices + *(index_itr++) * 3;
        const float *v1_itr = model->mesh->vertices + *(index_itr++) * 3;
        const float *v2_itr = model->mesh->vertices + *(index_itr++) * 3;

        Vec4 p0_v4 = { *(v0_itr++), *(v0_itr++), *(v0_itr), 1.0f };
        Vec4 p1_v4 = { *(v1_itr++), *(v1_itr++), *(v1_itr), 1.0f };
        Vec4 p2_v4 = { *(v2_itr++), *(v2_itr++), *(v2_itr), 1.0f };

        p0_v4 = mat4_transform(&transform, &p0_v4);
        p1_v4 = mat4_transform(&transform, &p1_v4);
        p2_v4 = mat4_transform(&transform, &p2_v4);

        Vec3 p0 = { p0_v4.x, p0_v4.y, p0_v4.z };
        Vec3 p1 = { p1_v4.x, p1_v4.y, p1_v4.z };
        Vec3 p2 = { p2_v4.x, p2_v4.y, p2_v4.z };

        Vec3 edge1 = vec3_sub(&p1, &p0);
        Vec3 edge2 = vec3_sub(&p2, &p0);
        Vec3 N = vec3_cross(&edge1, &edge2);
        N = vec3_normalize(&N);

        Collision_Result res_tri = __collide_sphere_with_triangle_impl(sphere, position, player, p0, p1, p2, rad_sqr, N);
        if (res_tri.collision_depth > result.collision_depth)
        {
            result = res_tri;
            result.hit_ground = result.displacement.y > 0;
        }
    }

    return result;
}

Collision_Result collide_capsule_with_static(const Capsule *capsule, const Model *model, const Player *player, float dt)
{
    Collision_Result result;
    result.displacement.x = 0.0f;
    result.displacement.y = 0.0f;
    result.displacement.z = 0.0f;
    result.res_velocity = player->velocity;
    result.collision_depth = 0.0f;
    result.hit_ground = IC_FALSE;

    float rad_sqr = capsule->radius * capsule->radius;

    Mat4 transform = model_transform_matrix(model);
    Vec3 position = vec3_add(&player->position, &player->collidable_offset);

    Vec3 cap_norm = { 0.0f, 1.0f, 0.0f };
    Vec3 A = position;
    A.y -= capsule->body_height / 2;
    Vec3 B = position;
    B.y += capsule->body_height / 2;

    Vec3 base = A;
    base.y -= capsule->radius;
    Vec3 tip = B;
    tip.y += capsule->radius;

    uint32_t num_indices = model->mesh->num_indices / 3;
    const uint32_t *index_itr = model->mesh->indices;

    for (uint32_t i = 0; i < num_indices; ++i)
    {
        const float *v0_itr = model->mesh->vertices + *(index_itr++) * 3;
        const float *v1_itr = model->mesh->vertices + *(index_itr++) * 3;
        const float *v2_itr = model->mesh->vertices + *(index_itr++) * 3;

        Vec4 p0_v4 = { *(v0_itr++), *(v0_itr++), *(v0_itr), 1.0f };
        Vec4 p1_v4 = { *(v1_itr++), *(v1_itr++), *(v1_itr), 1.0f };
        Vec4 p2_v4 = { *(v2_itr++), *(v2_itr++), *(v2_itr), 1.0f };

        p0_v4 = mat4_transform(&transform, &p0_v4);
        p1_v4 = mat4_transform(&transform, &p1_v4);
        p2_v4 = mat4_transform(&transform, &p2_v4);

        Vec3 p0 = { p0_v4.x, p0_v4.y, p0_v4.z };
        Vec3 p1 = { p1_v4.x, p1_v4.y, p1_v4.z };
        Vec3 p2 = { p2_v4.x, p2_v4.y, p2_v4.z };

        Vec3 edge1 = vec3_sub(&p1, &p0);
        Vec3 edge2 = vec3_sub(&p2, &p0);
        Vec3 N = vec3_cross(&edge1, &edge2);
        N = vec3_normalize(&N);

        Vec3 base_p0 = vec3_sub(&p0, &base);
        Vec3 base_p0_scaled = vec3_scale(&base_p0, 1.0f / vec3_dot(&N, &cap_norm));
        float t = vec3_dot(&N, &base_p0_scaled);

        Vec3 cap_norm_scaled = vec3_scale(&cap_norm, t);
        Vec3 line_plane_intersection = vec3_add(&base, &cap_norm_scaled);

        Vec3 ref_point;

        Vec3 p0_lpi = vec3_sub(&line_plane_intersection, &p0);
        Vec3 p1_lpi = vec3_sub(&line_plane_intersection, &p1);
        Vec3 p2_lpi = vec3_sub(&line_plane_intersection, &p2);
        Vec3 p0_p1 = vec3_sub(&p1, &p0);
        Vec3 p1_p2 = vec3_sub(&p2, &p1);
        Vec3 p2_p0 = vec3_sub(&p0, &p2);

        Vec3 c0 = vec3_cross(&p0_lpi, &p0_p1);
        Vec3 c1 = vec3_cross(&p1_lpi, &p1_p2);
        Vec3 c2 = vec3_cross(&p2_lpi, &p2_p0);
        IC_BOOL inside = vec3_dot(&c0, &N) <= 0 && vec3_dot(&c1, &N) <= 0 && vec3_dot(&c2, &N) <= 0;

        if (inside)
        {
            ref_point = line_plane_intersection;
        }
        else
        {
            float min_dist = INFINITY;

            Vec3 projected_edge0 = closest_to_line(&p0, &p1, &line_plane_intersection);
            Vec3 v0 = vec3_sub(&line_plane_intersection, &projected_edge0);
            float dist_sqr0 = vec3_length_sqr(&v0);
            if (dist_sqr0 < min_dist)
            {
                min_dist = dist_sqr0;
                ref_point = projected_edge0;
            }

            Vec3 projected_edge1 = closest_to_line(&p1, &p2, &line_plane_intersection);
            Vec3 v1 = vec3_sub(&line_plane_intersection, &projected_edge1);
            float dist_sqr1 = vec3_length_sqr(&v1);
            if (dist_sqr1 < min_dist)
            {
                min_dist = dist_sqr1;
                ref_point = projected_edge1;
            }

            Vec3 projected_edge2 = closest_to_line(&p2, &p0, &line_plane_intersection);
            Vec3 v2 = vec3_sub(&line_plane_intersection, &projected_edge2);
            float dist_sqr2 = vec3_length_sqr(&v2);
            if (dist_sqr2 < min_dist)
            {
                min_dist = dist_sqr2;
                ref_point = projected_edge2;
            }
        }

        Vec3 center = closest_to_line(&A, &B, &ref_point);
        Sphere sphere = { capsule->radius };

        Collision_Result res_tri = __collide_sphere_with_triangle_impl(&sphere, center, player, p0, p1, p2, rad_sqr, N);
        if (res_tri.collision_depth > result.collision_depth)
        {
            result = res_tri;
            result.hit_ground = (center.x == A.x && center.y == A.y && center.z == A.z);
        }
    }

    return result;
}