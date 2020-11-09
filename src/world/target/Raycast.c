#include"world/target/Raycast.h"

#include"world/target/Target.h"
#include"world/model/mesh/Mesh.h"

#include<math.h>

RaycastResult raycast_check_target(const Vec3 *ray_origin, const Vec3 *ray_direction, float ray_length, const Target *target)
{
    RaycastResult res =
    {
        .collision = IC_FALSE,
        .distance = INFINITY
    };
    
    Mat4 transform = model_transform_matrix(&target->model);

    uint32_t num_indices = target->model.mesh->num_indices / 3;
    const uint32_t *index_itr = target->model.mesh->indices;

    for (uint32_t i = 0; i < num_indices; ++i)
    {
        const float *v0_itr = target->model.mesh->vertices + *(index_itr++) * 3;
        const float *v1_itr = target->model.mesh->vertices + *(index_itr++) * 3;
        const float *v2_itr = target->model.mesh->vertices + *(index_itr++) * 3;

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

        Vec3 line_plane = vec3_sub(&p0, ray_origin);
        float line_normal = vec3_dot(ray_direction, &N);
        if (line_normal == 0) continue; // no intersection

        float ray_dist = vec3_dot(&line_plane, &N) / line_normal;

        if (ray_dist > ray_length || ray_dist < 0) continue;

        Vec3 intersection = vec3_scale(ray_direction, ray_dist);
        intersection = vec3_add(ray_origin, &intersection);

        Vec3 p0_p1 = vec3_sub(&p1, &p0);
        Vec3 p1_p2 = vec3_sub(&p2, &p1);
        Vec3 p2_p0 = vec3_sub(&p0, &p2);

        Vec3 p0_int = vec3_sub(&intersection, &p0);
        Vec3 p1_int = vec3_sub(&intersection, &p1);
        Vec3 p2_int = vec3_sub(&intersection, &p2);

        Vec3 c0 = vec3_cross(&p0_int, &p0_p1);
        Vec3 c1 = vec3_cross(&p1_int, &p1_p2);
        Vec3 c2 = vec3_cross(&p2_int, &p2_p0);

        if (vec3_dot(&c0, &N) <= 0 && vec3_dot(&c1, &N) <= 0 && vec3_dot(&c2, &N) <= 0 && ray_dist < res.distance)
        {
            res.collision = IC_TRUE;
            res.intersection = intersection;
            res.distance = ray_dist;
        }
    }

    return res;
}