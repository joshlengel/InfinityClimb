#pragma once

#include"util/math/Vec.h"

struct _Mat3
{
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
};

struct _Mat4
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
};

typedef struct _Mat3 Mat3;
typedef struct _Mat4 Mat4;

Mat3 mat3_identity();
Mat4 mat4_identity();

Vec3 mat3_transform(const Mat3 *m, const Vec3 *v);
Vec4 mat4_transform(const Mat4 *m, const Vec4 *v);

Mat3 mat3_mul(const Mat3 *m1, const Mat3 *m2);
Mat4 mat4_mul(const Mat4 *m1, const Mat4 *m2);

void mat3_load(const Mat3 *m, float *buffer);
void mat4_load(const Mat4 *m, float *buffer);

Mat4 mat4_make_scale(float sx, float sy, float sz);
Mat4 mat4_make_translate(float tx, float ty, float tz);
Mat4 mat4_make_project(float fov, float aspect_ratio, float clip_near, float clip_far);