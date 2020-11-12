#include"util/math/Vec.h"

#include<math.h>

Vec2 vec2_add(const Vec2 *v1, const Vec2 *v2)
{
    Vec2 res = { v1->x + v2->x, v1->y + v2->y }; return res;
}

Vec3 vec3_add(const Vec3 *v1, const Vec3 *v2)
{
    Vec3 res = { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z }; return res;
}

Vec4 vec4_add(const Vec4 *v1, const Vec4 *v2)
{
    Vec4 res = { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z, v1->w + v2->w }; return res;
}

Vec2 vec2_sub(const Vec2 *v1, const Vec2 *v2)
{
    Vec2 res = { v1->x - v2->x, v1->y - v2->y }; return res;
}

Vec3 vec3_sub(const Vec3 *v1, const Vec3 *v2)
{
    Vec3 res = { v1->x - v2->x, v1->y - v2->y, v1->z - v2->z }; return res;
}

Vec4 vec4_sub(const Vec4 *v1, const Vec4 *v2)
{
    Vec4 res = { v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, v1->w - v2->w }; return res;
}

Vec2 vec2_scale(const Vec2 *v, float scalar)
{
    Vec2 res = { v->x * scalar, v->y * scalar }; return res;
}

Vec3 vec3_scale(const Vec3 *v, float scalar)
{
    Vec3 res = { v->x * scalar, v->y * scalar, v->z * scalar }; return res;
}

Vec4 vec4_scale(const Vec4 *v, float scalar)
{
    Vec4 res = { v->x * scalar, v->y * scalar, v->z * scalar, v->w * scalar }; return res;
}

Vec2 vec2_negate(const Vec2 *v)
{
    Vec2 res = { -v->x, -v->y }; return res;
}

Vec3 vec3_negate(const Vec3 *v)
{
    Vec3 res = { -v->x, -v->y, -v->z }; return res;
}

Vec4 vec4_negate(const Vec4 *v)
{
    Vec4 res = { -v->x, -v->y, -v->z, -v->w }; return res;
}

static float __abs_impl(float v)
{
    return v < 0.0f? -v : v;
}

Vec2 vec2_abs(const Vec2 *v)
{
    Vec2 res = { __abs_impl(v->x), __abs_impl(v->y) }; return res;
}

Vec3 vec3_abs(const Vec3 *v)
{
    Vec3 res = { __abs_impl(v->x), __abs_impl(v->y), __abs_impl(v->z) }; return res;
}

Vec4 vec4_abs(const Vec4 *v)
{
    Vec4 res = { __abs_impl(v->x), __abs_impl(v->y), __abs_impl(v->z), __abs_impl(v->w) }; return res;
}

float vec2_dot(const Vec2 *v1, const Vec2 *v2)
{
    return v1->x * v2->x + v1->y * v2->y;
}

float vec3_dot(const Vec3 *v1, const Vec3 *v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float vec4_dot(const Vec4 *v1, const Vec4 *v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;
}

float vec2_length_sqr(const Vec2 *v)
{
    return vec2_dot(v, v);
}

float vec3_length_sqr(const Vec3 *v)
{
    return vec3_dot(v, v);
}

float vec4_length_sqr(const Vec4 *v)
{
    return vec4_dot(v, v);
}

float vec2_length(const Vec2 *v)
{
    return sqrtf(vec2_length_sqr(v));
}

float vec3_length(const Vec3 *v)
{
    return sqrtf(vec3_length_sqr(v));
}

float vec4_length(const Vec4 *v)
{
    return sqrtf(vec4_length_sqr(v));
}

Vec2 vec2_normalize(const Vec2 *v)
{
    float len = vec2_length(v);

    if (!len)
    {
        Vec2 res = { 0.0f, 0.0f }; return res;
    }

    return vec2_scale(v, 1.0f / len);
}

Vec3 vec3_normalize(const Vec3 *v)
{
    float len = vec3_length(v);

    if (!len)
    {
        Vec3 res = { 0.0f, 0.0f, 0.0f }; return res;
    }

    return vec3_scale(v, 1.0f / len);
}

Vec4 vec4_normalize(const Vec4 *v)
{
    float len = vec4_length(v);

    if (!len)
    {
        Vec4 res = { 0.0f, 0.0f, 0.0f, 0.0f }; return res;
    }

    return vec4_scale(v, 1.0f / len);
}

Vec3 vec3_cross(const Vec3 *v1, const Vec3 *v2)
{
    Vec3 res = { v1->y * v2->z - v1->z * v2->y, v1->z * v2->x - v1->x * v2->z, v1->x * v2->y - v1->y * v2->x };
    return res;
}