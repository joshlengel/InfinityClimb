#include"math/Mat.h"

#include<math.h>

Mat3 mat3_identity()
{
    Mat3 res =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    return res;
}

Mat4 mat4_identity()
{
    Mat4 res =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return res;
}

Vec3 mat3_transform(const Mat3 *m, const Vec3 *v)
{
    Vec3 res =
    {
        v->x * m->m00 + v->y * m->m10 + v->z * m->m20,
        v->x * m->m01 + v->y * m->m11 + v->z * m->m21,
        v->x * m->m02 + v->y * m->m12 + v->z * m->m22
    };
    return res;
}

Vec4 mat4_transform(const Mat4 *m, const Vec4 *v)
{
    Vec4 res =
    {
        v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + v->w * m->m30,
        v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + v->w * m->m31,
        v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + v->w * m->m32,
        v->x * m->m03 + v->y * m->m13 + v->z * m->m23 + v->w * m->m33
    };
    return res;
}

Mat3 mat3_mul(const Mat3 *m1, const Mat3 *m2)
{
    Mat3 res =
    {
        m1->m00 * m2->m00 + m1->m01 * m2->m10 + m1->m02 * m2->m20, m1->m10 * m2->m00 + m1->m11 * m2->m10 + m1->m12 * m2->m20, m1->m20 * m2->m00 + m1->m21 * m2->m10 + m1->m22 * m2->m20,
        m1->m00 * m2->m01 + m1->m01 * m2->m11 + m1->m02 * m2->m21, m1->m10 * m2->m01 + m1->m11 * m2->m11 + m1->m12 * m2->m21, m1->m20 * m2->m01 + m1->m21 * m2->m11 + m1->m22 * m2->m21,
        m1->m00 * m2->m02 + m1->m01 * m2->m12 + m1->m02 * m2->m22, m1->m10 * m2->m02 + m1->m11 * m2->m12 + m1->m12 * m2->m22, m1->m20 * m2->m02 + m1->m21 * m2->m12 + m1->m22 * m2->m22
    };
    return res;
}

Mat4 mat4_mul(const Mat4 *m1, const Mat4 *m2)
{
    Mat4 res =
    {
        m1->m00 * m2->m00 + m1->m01 * m2->m10 + m1->m02 * m2->m20 + m1->m03 * m2->m30, m1->m10 * m2->m00 + m1->m11 * m2->m10 + m1->m12 * m2->m20 + m1->m13 * m2->m30, m1->m20 * m2->m00 + m1->m21 * m2->m10 + m1->m22 * m2->m20 + m1->m23 * m2->m30, m1->m30 * m2->m00 + m1->m31 * m2->m10 + m1->m32 * m2->m20 + m1->m33 * m2->m30,
        m1->m00 * m2->m01 + m1->m01 * m2->m11 + m1->m02 * m2->m21 + m1->m03 * m2->m31, m1->m10 * m2->m01 + m1->m11 * m2->m11 + m1->m12 * m2->m21 + m1->m13 * m2->m31, m1->m20 * m2->m01 + m1->m21 * m2->m11 + m1->m22 * m2->m21 + m1->m23 * m2->m31, m1->m30 * m2->m01 + m1->m31 * m2->m11 + m1->m32 * m2->m21 + m1->m33 * m2->m31,
        m1->m00 * m2->m02 + m1->m01 * m2->m12 + m1->m02 * m2->m22 + m1->m03 * m2->m32, m1->m10 * m2->m02 + m1->m11 * m2->m12 + m1->m12 * m2->m22 + m1->m13 * m2->m32, m1->m20 * m2->m02 + m1->m21 * m2->m12 + m1->m22 * m2->m22 + m1->m23 * m2->m32, m1->m30 * m2->m02 + m1->m31 * m2->m12 + m1->m32 * m2->m22 + m1->m33 * m2->m32,
        m1->m00 * m2->m03 + m1->m01 * m2->m13 + m1->m02 * m2->m23 + m1->m03 * m2->m33, m1->m10 * m2->m03 + m1->m11 * m2->m13 + m1->m12 * m2->m23 + m1->m13 * m2->m33, m1->m20 * m2->m03 + m1->m21 * m2->m13 + m1->m22 * m2->m23 + m1->m23 * m2->m33, m1->m30 * m2->m03 + m1->m31 * m2->m13 + m1->m32 * m2->m23 + m1->m33 * m2->m33
    };
    return res;
}

void mat3_load(const Mat3 *m, float *buffer)
{
    *(buffer++) = m->m00; *(buffer++) = m->m10; *(buffer++) = m->m20;
    *(buffer++) = m->m01; *(buffer++) = m->m11; *(buffer++) = m->m21;
    *(buffer++) = m->m02; *(buffer++) = m->m12; *(buffer++) = m->m22;
}

void mat4_load(const Mat4 *m, float *buffer)
{
    *(buffer++) = m->m00; *(buffer++) = m->m10; *(buffer++) = m->m20; *(buffer++) = m->m30;
    *(buffer++) = m->m01; *(buffer++) = m->m11; *(buffer++) = m->m21; *(buffer++) = m->m31;
    *(buffer++) = m->m02; *(buffer++) = m->m12; *(buffer++) = m->m22; *(buffer++) = m->m32;
    *(buffer++) = m->m03; *(buffer++) = m->m13; *(buffer++) = m->m23; *(buffer++) = m->m33;
}

Mat4 mat4_make_scale(float sx, float sy, float sz)
{
    Mat4 res =
    {
          sx, 0.0f, 0.0f, 0.0f,
        0.0f,   sy, 0.0f, 0.0f,
        0.0f, 0.0f,   sz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return res;
}

Mat4 mat4_make_translate(float tx, float ty, float tz)
{
    Mat4 res =
    {
        1.0f, 0.0f, 0.0f,   tx,
        0.0f, 1.0f, 0.0f,   ty,
        0.0f, 0.0f, 1.0f,   tz,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return res;
}

Mat4 mat4_make_project(float fov, float aspect_ratio, float clip_near, float clip_far)
{
    float tan_half_fov = tanf(fov * 0.5f);

    Mat4 res = mat4_identity();

    if (aspect_ratio > 1.0f)
    {
        res.m00 = 1.0f / tan_half_fov;
        res.m11 = aspect_ratio / tan_half_fov;
    }
    else
    {
        res.m00 = 1.0f / (tan_half_fov * aspect_ratio);
        res.m11 = 1.0f / tan_half_fov;
    }
    
    res.m22 = (clip_far + clip_near) / (clip_far - clip_near);
    res.m32 = -2.0f * clip_far * clip_near / (clip_far - clip_near);
    res.m23 = 1.0f;
    res.m33 = 0.0f;

    return res;
}