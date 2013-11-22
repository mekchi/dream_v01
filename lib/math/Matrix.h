
/* mekchi */

#ifndef _MEKCHI_MATRIX_
#define _MEKCHI_MATRIX_

#include <string.h>
#include <math.h>

#include "Types.h"
#include "FastMath.h"

#ifdef __cplusplus
extern "C"
{
#endif

// 0 4 8 12
// 1 5 9 13
// 2 6 10 14
// 3 7 11 15
    
typedef union
{
    struct
    {
        float_t m00, m01, m02, m03;
        float_t m10, m11, m12, m13;
        float_t m20, m21, m22, m23;
        float_t m30, m31, m32, m33;
    };
    float_t m[16];
} matrix44;
    

static inline matrix44 mSetPerspectiveBasic(float_t right, float_t top, float_t near, float_t far);
static inline matrix44 mSetPerspective(float_t fovyRadians, float_t aspect, float_t nearZ, float_t farZ);
static inline void mCopy(matrix44 *a, matrix44 *b);
static inline void mSetIdentity(matrix44 *m);
//static inline void mSetTranslation(matrix44 *m, vector3* v);
static inline void mSetTranslation(matrix44 *m, float_t x, float_t y, float_t z);
static inline void mSetRotation(matrix44* m, float_t angle, float_t x, float_t y, float_t z);
static inline void mMultiplyToR(matrix44 *left, matrix44 *right, matrix44 *result);
static inline void mMultiply(matrix44 *left, matrix44 *right);
static inline matrix44 mSetOrthographic(float_t left, float_t right, float_t bottom, float_t top, float_t near, float_t far);
static inline void mTranspose(matrix44 *m);
static inline float_t mDeterminant(matrix44 *m);
static inline void mScale(matrix44 *m, float_t s);
static inline void mInvert(matrix44 *m);
static inline void mInvertToR(matrix44 *m, matrix44 *result);
    
    

// implemetation
    
static inline void mCopy(matrix44 *a, matrix44 *b)
{
    memcpy(a, b, sizeof(matrix44));
}
    
static inline void mSetIdentity(matrix44 *m)
{
    m->m[0] = 1.0f;
    m->m[5] = 1.0f;
    m->m[10] = 1.0f;
    m->m[15] = 1.0f;
    m->m[1] = 0.0f;
    m->m[2] = 0.0f;
    m->m[3] = 0.0f;
    m->m[4] = 0.0f;
    m->m[6] = 0.0f;
    m->m[7] = 0.0f;
    m->m[8] = 0.0f;
    m->m[9] = 0.0f;
    m->m[11] = 0.0f;
    m->m[12] = 0.0f;
    m->m[13] = 0.0f;
    m->m[14] = 0.0f;
}
    
static inline matrix44 mSetPerspective(float_t fovyRadians, float_t aspect, float_t nearZ, float_t farZ)
{
    float_t cotan = 1.0f / tanf(fovyRadians * 0.5f);
    
    matrix44 m = { cotan / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, cotan, 0.0f, 0.0f,
        0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), -1.0f,
        0.0f, 0.0f, (2.0f * farZ * nearZ) / (nearZ - farZ), 0.0f };
    
    return m;
}
    
static inline matrix44 mSetPerspectiveBasic(float_t right, float_t top, float_t near, float_t far)
{
    // 0 4 8 12
    // 1 5 9 13
    // 2 6 10 14
    // 3 7 11 15
    
    matrix44 m = {near / right, 0.0f, 0.0f, 0.0f,
        0.0f, near / top, 0.0f, 0.0f,
        0.0f, 0.0f, -(far + near) / (far - near), -1.0f,
        0.0f, 0.0f, -2.0f * far * near / (far - near), 0.0f};
    
    return m;
}

static inline matrix44 mSetOrthographic(float_t left, float_t right, float_t bottom, float_t top, float_t near, float_t far)
{
    float_t ral = right + left;
    float_t rsl = right - left;
    float_t tab = top + bottom;
    float_t tsb = top - bottom;
    float_t fan = far + near;
    float_t fsn = far - near;
    
    matrix44 m = { 2.0f / rsl, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / tsb, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / fsn, 0.0f,
        -ral / rsl, -tab / tsb, -fan / fsn, 1.0f };
    
    return m;
}
    
static inline void mSetTranslation(matrix44 *m, float_t x, float_t y, float_t z)
{
    m->m[12] = x;
    m->m[13] = y;
    m->m[14] = z;
    m->m[0] = 1.0f;
    m->m[5] = 1.0f;
    m->m[10] = 1.0f;
    m->m[15] = 1.0f;
    m->m[1] = 0.0f;
    m->m[2] = 0.0f;
    m->m[3] = 0.0f;
    m->m[4] = 0.0f;
    m->m[6] = 0.0f;
    m->m[7] = 0.0f;
    m->m[8] = 0.0f;
    m->m[9] = 0.0f;
    m->m[11] = 0.0f;
}
    
//static inline void mSetTranslation(matrix44 *m, vector3* v)
//{
//    m->m[12] = v->x;
//    m->m[13] = v->y;
//    m->m[14] = v->z;
//}
    
static inline void mSetScale(matrix44* m, float_t x, float_t y, float_t z)
{
    m->m[0] = x;
    m->m[5] = y;
    m->m[10] = z;
}
    
static inline void mSetRotation(matrix44* m, float_t angle, float_t x, float_t y, float_t z)
{
    float_t length = fastSqrt(x * x + y * y + z * z);
    float_t xp = x / length;
    float_t yp = y / length;
    float_t zp = z / length;
    float cos = cosf(DEGREE_TO_RADIAN(angle));
    float cosp = 1.0f - cos;
    float sin = sinf(DEGREE_TO_RADIAN(angle));
    
    m->m[0] = cos + cosp * xp * xp;
    m->m[4] = cosp * xp * yp + zp * sin;
    m->m[8] = cosp * xp * zp - yp * sin;
    m->m[1] = cosp * xp * yp - zp * sin;
    m->m[5] = cos + cosp * yp * yp;
    m->m[9] = cosp * yp * zp + xp * sin;
    m->m[2] = cosp * xp * zp + yp * sin;
    m->m[6] = cosp * yp * zp - xp * sin;
    m->m[10] = cos + cosp * zp * zp;
}

static inline void mMultiplyToR(matrix44 *left, matrix44 *right, matrix44 *result)
{
    result->m[0]  = left->m[0] * right->m[0]  + left->m[4] * right->m[1]  + left->m[8] * right->m[2]   + left->m[12] * right->m[3];
    result->m[4]  = left->m[0] * right->m[4]  + left->m[4] * right->m[5]  + left->m[8] * right->m[6]   + left->m[12] * right->m[7];
    result->m[8]  = left->m[0] * right->m[8]  + left->m[4] * right->m[9]  + left->m[8] * right->m[10]  + left->m[12] * right->m[11];
    result->m[12] = left->m[0] * right->m[12] + left->m[4] * right->m[13] + left->m[8] * right->m[14]  + left->m[12] * right->m[15];
    
    result->m[1]  = left->m[1] * right->m[0]  + left->m[5] * right->m[1]  + left->m[9] * right->m[2]   + left->m[13] * right->m[3];
    result->m[5]  = left->m[1] * right->m[4]  + left->m[5] * right->m[5]  + left->m[9] * right->m[6]   + left->m[13] * right->m[7];
    result->m[9]  = left->m[1] * right->m[8]  + left->m[5] * right->m[9]  + left->m[9] * right->m[10]  + left->m[13] * right->m[11];
    result->m[13] = left->m[1] * right->m[12] + left->m[5] * right->m[13] + left->m[9] * right->m[14]  + left->m[13] * right->m[15];
    
    result->m[2]  = left->m[2] * right->m[0]  + left->m[6] * right->m[1]  + left->m[10] * right->m[2]  + left->m[14] * right->m[3];
    result->m[6]  = left->m[2] * right->m[4]  + left->m[6] * right->m[5]  + left->m[10] * right->m[6]  + left->m[14] * right->m[7];
    result->m[10] = left->m[2] * right->m[8]  + left->m[6] * right->m[9]  + left->m[10] * right->m[10] + left->m[14] * right->m[11];
    result->m[14] = left->m[2] * right->m[12] + left->m[6] * right->m[13] + left->m[10] * right->m[14] + left->m[14] * right->m[15];
    
    result->m[3]  = left->m[3] * right->m[0]  + left->m[7] * right->m[1]  + left->m[11] * right->m[2]  + left->m[15] * right->m[3];
    result->m[7]  = left->m[3] * right->m[4]  + left->m[7] * right->m[5]  + left->m[11] * right->m[6]  + left->m[15] * right->m[7];
    result->m[11] = left->m[3] * right->m[8]  + left->m[7] * right->m[9]  + left->m[11] * right->m[10] + left->m[15] * right->m[11];
    result->m[15] = left->m[3] * right->m[12] + left->m[7] * right->m[13] + left->m[11] * right->m[14] + left->m[15] * right->m[15];
}
    
static inline void mMultiply(matrix44 *left, matrix44 *right)
{
    matrix44 result;
    
    result.m[0]  = left->m[0] * right->m[0]  + left->m[4] * right->m[1]  + left->m[8] * right->m[2]   + left->m[12] * right->m[3];
    result.m[4]  = left->m[0] * right->m[4]  + left->m[4] * right->m[5]  + left->m[8] * right->m[6]   + left->m[12] * right->m[7];
    result.m[8]  = left->m[0] * right->m[8]  + left->m[4] * right->m[9]  + left->m[8] * right->m[10]  + left->m[12] * right->m[11];
    result.m[12] = left->m[0] * right->m[12] + left->m[4] * right->m[13] + left->m[8] * right->m[14]  + left->m[12] * right->m[15];
    
    result.m[1]  = left->m[1] * right->m[0]  + left->m[5] * right->m[1]  + left->m[9] * right->m[2]   + left->m[13] * right->m[3];
    result.m[5]  = left->m[1] * right->m[4]  + left->m[5] * right->m[5]  + left->m[9] * right->m[6]   + left->m[13] * right->m[7];
    result.m[9]  = left->m[1] * right->m[8]  + left->m[5] * right->m[9]  + left->m[9] * right->m[10]  + left->m[13] * right->m[11];
    result.m[13] = left->m[1] * right->m[12] + left->m[5] * right->m[13] + left->m[9] * right->m[14]  + left->m[13] * right->m[15];
    
    result.m[2]  = left->m[2] * right->m[0]  + left->m[6] * right->m[1]  + left->m[10] * right->m[2]  + left->m[14] * right->m[3];
    result.m[6]  = left->m[2] * right->m[4]  + left->m[6] * right->m[5]  + left->m[10] * right->m[6]  + left->m[14] * right->m[7];
    result.m[10] = left->m[2] * right->m[8]  + left->m[6] * right->m[9]  + left->m[10] * right->m[10] + left->m[14] * right->m[11];
    result.m[14] = left->m[2] * right->m[12] + left->m[6] * right->m[13] + left->m[10] * right->m[14] + left->m[14] * right->m[15];
    
    result.m[3]  = left->m[3] * right->m[0]  + left->m[7] * right->m[1]  + left->m[11] * right->m[2]  + left->m[15] * right->m[3];
    result.m[7]  = left->m[3] * right->m[4]  + left->m[7] * right->m[5]  + left->m[11] * right->m[6]  + left->m[15] * right->m[7];
    result.m[11] = left->m[3] * right->m[8]  + left->m[7] * right->m[9]  + left->m[11] * right->m[10] + left->m[15] * right->m[11];
    result.m[15] = left->m[3] * right->m[12] + left->m[7] * right->m[13] + left->m[11] * right->m[14] + left->m[15] * right->m[15];
    
    (*left) = result;
}

static inline void mScale(matrix44 *m, float_t s)
{
    int i;
    
    for(i = 0; i < 16; i++)
        m->m[i] *= s;
}
    
static inline void mInvert(matrix44 *m)
{
    matrix44 temp;
    
    temp.m00 = (m->m12 * m->m23 * m->m31) - (m->m13 * m->m22 * m->m31) + (m->m13 * m->m21 * m->m32) - (m->m11 * m->m23 * m->m32) - (m->m12 * m->m21 * m->m33) + (m->m11 * m->m22 * m->m33);
    temp.m01 = (m->m03 * m->m22 * m->m31) - (m->m02 * m->m23 * m->m31) - (m->m03 * m->m21 * m->m32) + (m->m01 * m->m23 * m->m32) + (m->m02 * m->m21 * m->m33) - (m->m01 * m->m22 * m->m33);
    temp.m02 = (m->m02 * m->m13 * m->m31) - (m->m03 * m->m12 * m->m31) + (m->m03 * m->m11 * m->m32) - (m->m01 * m->m13 * m->m32) - (m->m02 * m->m11 * m->m33) + (m->m01 * m->m12 * m->m33);
    temp.m03 = (m->m03 * m->m12 * m->m21) - (m->m02 * m->m13 * m->m21) - (m->m03 * m->m11 * m->m22) + (m->m01 * m->m13 * m->m22) + (m->m02 * m->m11 * m->m23) - (m->m01 * m->m12 * m->m23);
    temp.m10 = (m->m13 * m->m22 * m->m30) - (m->m12 * m->m23 * m->m30) - (m->m13 * m->m20 * m->m32) + (m->m10 * m->m23 * m->m32) + (m->m12 * m->m20 * m->m33) - (m->m10 * m->m22 * m->m33);
    temp.m11 = (m->m02 * m->m23 * m->m30) - (m->m03 * m->m22 * m->m30) + (m->m03 * m->m20 * m->m32) - (m->m00 * m->m23 * m->m32) - (m->m02 * m->m20 * m->m33) + (m->m00 * m->m22 * m->m33);
    temp.m12 = (m->m03 * m->m12 * m->m30) - (m->m02 * m->m13 * m->m30) - (m->m03 * m->m10 * m->m32) + (m->m00 * m->m13 * m->m32) + (m->m02 * m->m10 * m->m33) - (m->m00 * m->m12 * m->m33);
    temp.m13 = (m->m02 * m->m13 * m->m20) - (m->m03 * m->m12 * m->m20) + (m->m03 * m->m10 * m->m22) - (m->m00 * m->m13 * m->m22) - (m->m02 * m->m10 * m->m23) + (m->m00 * m->m12 * m->m23);
    temp.m20 = (m->m11 * m->m23 * m->m30) - (m->m13 * m->m21 * m->m30) + (m->m13 * m->m20 * m->m31) - (m->m10 * m->m23 * m->m31) - (m->m11 * m->m20 * m->m33) + (m->m10 * m->m21 * m->m33);
    temp.m21 = (m->m03 * m->m21 * m->m30) - (m->m01 * m->m23 * m->m30) - (m->m03 * m->m20 * m->m31) + (m->m00 * m->m23 * m->m31) + (m->m01 * m->m20 * m->m33) - (m->m00 * m->m21 * m->m33);
    temp.m22 = (m->m01 * m->m13 * m->m30) - (m->m03 * m->m11 * m->m30) + (m->m03 * m->m10 * m->m31) - (m->m00 * m->m13 * m->m31) - (m->m01 * m->m10 * m->m33) + (m->m00 * m->m11 * m->m33);
    temp.m23 = (m->m03 * m->m11 * m->m20) - (m->m01 * m->m13 * m->m20) - (m->m03 * m->m10 * m->m21) + (m->m00 * m->m13 * m->m21) + (m->m01 * m->m10 * m->m23) - (m->m00 * m->m11 * m->m23);
    temp.m30 = (m->m12 * m->m21 * m->m30) - (m->m11 * m->m22 * m->m30) - (m->m12 * m->m20 * m->m31) + (m->m10 * m->m22 * m->m31) + (m->m11 * m->m20 * m->m32) - (m->m10 * m->m21 * m->m32);
    temp.m31 = (m->m01 * m->m22 * m->m30) - (m->m02 * m->m21 * m->m30) + (m->m02 * m->m20 * m->m31) - (m->m00 * m->m22 * m->m31) - (m->m01 * m->m20 * m->m32) + (m->m00 * m->m21 * m->m32);
    temp.m32 = (m->m02 * m->m11 * m->m30) - (m->m01 * m->m12 * m->m30) - (m->m02 * m->m10 * m->m31) + (m->m00 * m->m12 * m->m31) + (m->m01 * m->m10 * m->m32) - (m->m00 * m->m11 * m->m32);
    temp.m33 = (m->m01 * m->m12 * m->m20) - (m->m02 * m->m11 * m->m20) + (m->m02 * m->m10 * m->m21) - (m->m00 * m->m12 * m->m21) - (m->m01 * m->m10 * m->m22) + (m->m00 * m->m11 * m->m22);

    mScale(&temp, (1.0f / mDeterminant(m)));
    memcpy(m, &temp, sizeof(matrix44));
}
    
static inline void mInvertToR(matrix44 *m, matrix44 *result)
{
    result->m00 = (m->m12 * m->m23 * m->m31) - (m->m13 * m->m22 * m->m31) + (m->m13 * m->m21 * m->m32) - (m->m11 * m->m23 * m->m32) - (m->m12 * m->m21 * m->m33) + (m->m11 * m->m22 * m->m33);
    result->m01 = (m->m03 * m->m22 * m->m31) - (m->m02 * m->m23 * m->m31) - (m->m03 * m->m21 * m->m32) + (m->m01 * m->m23 * m->m32) + (m->m02 * m->m21 * m->m33) - (m->m01 * m->m22 * m->m33);
    result->m02 = (m->m02 * m->m13 * m->m31) - (m->m03 * m->m12 * m->m31) + (m->m03 * m->m11 * m->m32) - (m->m01 * m->m13 * m->m32) - (m->m02 * m->m11 * m->m33) + (m->m01 * m->m12 * m->m33);
    result->m03 = (m->m03 * m->m12 * m->m21) - (m->m02 * m->m13 * m->m21) - (m->m03 * m->m11 * m->m22) + (m->m01 * m->m13 * m->m22) + (m->m02 * m->m11 * m->m23) - (m->m01 * m->m12 * m->m23);
    result->m10 = (m->m13 * m->m22 * m->m30) - (m->m12 * m->m23 * m->m30) - (m->m13 * m->m20 * m->m32) + (m->m10 * m->m23 * m->m32) + (m->m12 * m->m20 * m->m33) - (m->m10 * m->m22 * m->m33);
    result->m11 = (m->m02 * m->m23 * m->m30) - (m->m03 * m->m22 * m->m30) + (m->m03 * m->m20 * m->m32) - (m->m00 * m->m23 * m->m32) - (m->m02 * m->m20 * m->m33) + (m->m00 * m->m22 * m->m33);
    result->m12 = (m->m03 * m->m12 * m->m30) - (m->m02 * m->m13 * m->m30) - (m->m03 * m->m10 * m->m32) + (m->m00 * m->m13 * m->m32) + (m->m02 * m->m10 * m->m33) - (m->m00 * m->m12 * m->m33);
    result->m13 = (m->m02 * m->m13 * m->m20) - (m->m03 * m->m12 * m->m20) + (m->m03 * m->m10 * m->m22) - (m->m00 * m->m13 * m->m22) - (m->m02 * m->m10 * m->m23) + (m->m00 * m->m12 * m->m23);
    result->m20 = (m->m11 * m->m23 * m->m30) - (m->m13 * m->m21 * m->m30) + (m->m13 * m->m20 * m->m31) - (m->m10 * m->m23 * m->m31) - (m->m11 * m->m20 * m->m33) + (m->m10 * m->m21 * m->m33);
    result->m21 = (m->m03 * m->m21 * m->m30) - (m->m01 * m->m23 * m->m30) - (m->m03 * m->m20 * m->m31) + (m->m00 * m->m23 * m->m31) + (m->m01 * m->m20 * m->m33) - (m->m00 * m->m21 * m->m33);
    result->m22 = (m->m01 * m->m13 * m->m30) - (m->m03 * m->m11 * m->m30) + (m->m03 * m->m10 * m->m31) - (m->m00 * m->m13 * m->m31) - (m->m01 * m->m10 * m->m33) + (m->m00 * m->m11 * m->m33);
    result->m23 = (m->m03 * m->m11 * m->m20) - (m->m01 * m->m13 * m->m20) - (m->m03 * m->m10 * m->m21) + (m->m00 * m->m13 * m->m21) + (m->m01 * m->m10 * m->m23) - (m->m00 * m->m11 * m->m23);
    result->m30 = (m->m12 * m->m21 * m->m30) - (m->m11 * m->m22 * m->m30) - (m->m12 * m->m20 * m->m31) + (m->m10 * m->m22 * m->m31) + (m->m11 * m->m20 * m->m32) - (m->m10 * m->m21 * m->m32);
    result->m31 = (m->m01 * m->m22 * m->m30) - (m->m02 * m->m21 * m->m30) + (m->m02 * m->m20 * m->m31) - (m->m00 * m->m22 * m->m31) - (m->m01 * m->m20 * m->m32) + (m->m00 * m->m21 * m->m32);
    result->m32 = (m->m02 * m->m11 * m->m30) - (m->m01 * m->m12 * m->m30) - (m->m02 * m->m10 * m->m31) + (m->m00 * m->m12 * m->m31) + (m->m01 * m->m10 * m->m32) - (m->m00 * m->m11 * m->m32);
    result->m33 = (m->m01 * m->m12 * m->m20) - (m->m02 * m->m11 * m->m20) + (m->m02 * m->m10 * m->m21) - (m->m00 * m->m12 * m->m21) - (m->m01 * m->m10 * m->m22) + (m->m00 * m->m11 * m->m22);
    
    mScale(result, (1.0f / mDeterminant(m)));
}
    
static inline float_t mDeterminant(matrix44 *m)
{
    return ((m->m03 * m->m12 * m->m21 * m->m30) - (m->m02 * m->m13 * m->m21 * m->m30) - (m->m03 * m->m11 * m->m22 * m->m30) + (m->m01 * m->m13 * m->m22 * m->m30) + (m->m02 * m->m11 * m->m23 * m->m30) - (m->m01 * m->m12 * m->m23 * m->m30) - (m->m03 * m->m12 * m->m20 * m->m31) + (m->m02 * m->m13 * m->m20 * m->m31) + (m->m03 * m->m10 * m->m22 * m->m31) - (m->m00 * m->m13 * m->m22 * m->m31) - (m->m02 * m->m10 * m->m23 * m->m31) + (m->m00 * m->m12 * m->m23 * m->m31) + (m->m03 * m->m11 * m->m20 * m->m32) - (m->m01 * m->m13 * m->m20 * m->m32) - (m->m03 * m->m10 * m->m21 * m->m32) + (m->m00 * m->m13 * m->m21 * m->m32) + (m->m01 * m->m10 * m->m23 * m->m32) - (m->m00 * m->m11 * m->m23 * m->m32) - (m->m02 * m->m11 * m->m20 * m->m33) + (m->m01 * m->m12 * m->m20 * m->m33) + (m->m02 * m->m10 * m->m21 * m->m33) - (m->m00 * m->m12 * m->m21 * m->m33) - (m->m01 * m->m10 * m->m22 * m->m33) + (m->m00 * m->m11 * m->m22 * m->m33));
}
    

    
//void mMultiply_mv(vector3f *vector, matrix44f matrix)
//{
//    vector3f	temp;
//    float		r = m->m30 * vector->x + m->m31 * vector->y + m->m32 * vector->z + m->m33;
//    
//    temp.x = (m->m00 * vector->x + m->m01 * vector->y + m->m02 * vector->z + m->m03) / r;
//    temp.y = (m->m10 * vector->x + m->m11 * vector->y + m->m12 * vector->z + m->m13) / r;
//    temp.z = (m->m20 * vector->x + m->m21 * vector->y + m->m22 * vector->z + m->m23) / r;
//    memcpy(vector, &temp, SIZE_VPC3F);
//}

static inline void mTranspose(matrix44 *m)
{
    int i, j;
    matrix44 temp;
    
    memcpy(&temp, m, sizeof(matrix44));
    
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
        {
            m->m[(i * 4) + j] = temp.m[ i + (j * 4)];
        }
}
    
    
	
#ifdef __cplusplus
}
#endif

#endif
