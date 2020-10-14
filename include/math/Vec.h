#pragma once

typedef struct
{
    float x;
    float y;
} Vec2;

typedef struct
{
    float x;
    float y;
    float z;
} Vec3;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} Vec4;

Vec2 vec2_add(const Vec2 *v1, const Vec2 *v2);
Vec3 vec3_add(const Vec3 *v1, const Vec3 *v2);
Vec4 vec4_add(const Vec4 *v1, const Vec4 *v2);

Vec2 vec2_sub(const Vec2 *v1, const Vec2 *v2);
Vec3 vec3_sub(const Vec3 *v1, const Vec3 *v2);
Vec4 vec4_sub(const Vec4 *v1, const Vec4 *v2);

Vec2 vec2_scale(const Vec2 *v, float scalar);
Vec3 vec3_scale(const Vec3 *v, float scalar);
Vec4 vec4_scale(const Vec4 *v, float scalar);

Vec2 vec2_negate(const Vec2 *v);
Vec3 vec3_negate(const Vec3 *v);
Vec4 vec4_negate(const Vec4 *v);

float vec2_dot(const Vec2 *v1, const Vec2 *v2);
float vec3_dot(const Vec3 *v1, const Vec3 *v2);
float vec4_dot(const Vec4 *v1, const Vec4 *v2);

float vec2_length_sqr(const Vec2 *v);
float vec3_length_sqr(const Vec3 *v);
float vec4_length_sqr(const Vec4 *v);

float vec2_length(const Vec2 *v);
float vec3_length(const Vec3 *v);
float vec4_length(const Vec4 *v);

Vec2 vec2_normalize(const Vec2 *v);
Vec3 vec3_normalize(const Vec3 *v);
Vec4 vec4_normalize(const Vec4 *v);

Vec3 vec3_cross(const Vec3 *v1, const Vec3 *v2);