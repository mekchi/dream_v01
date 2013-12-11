//
//  VQMMath.h
//  dream
//
//  Created by Yermek Garifullanov on 11/23/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef dream_VQMMath_h
#define dream_VQMMath_h

#include <memory.h>
#include <math.h>

#include "FastMath.h"

// vector types

// vector 2D
typedef union
{
    struct
    {
        float x;
        float y;
    };
    float v[2];
} vector2;

// vector 3D
typedef union
{
    struct
    {
        float x;
        float y;
        float z;
    };
    float v[3];
} vector3;

// quaternion
typedef union
{
    struct
    {
        float x;
        float y;
        float z;
        float w;
    };
    float q[4];
} quaternion;

// matrix 4x4

typedef union
{
    struct
    {
        float m00, m01, m02, m03; // 0 4 8 12
        float m10, m11, m12, m13; // 1 5 9 13
        float m20, m21, m22, m23; // 2 6 10 14
        float m30, m31, m32, m33; // 3 7 11 15
    };
    float m[16];
} matrix44;


// methods

// vector 2D
void v2SetZero(vector2 *v);
void v2Copy(vector2 *to, vector2 *from);
void v2Set(vector2 *inOut, float x, float y);
void v2PerpendicularCCW(vector2 *v);
void v2PerpendicularCW(vector2 *v);

// vector 3D
void v3SetZero(vector3 *v);
void v3Copy(vector3 *inOut, vector3 *v);
void v3Set(vector3 *inOut, float x, float y, float z);
void v3Add(vector3 *inOut, vector3 *v);
void v3AddToR(vector3 *result, vector3 *a, vector3 *b);
void v3Subtract(vector3 *inOut, vector3 *v);
void v3SubtractToR(vector3 *result, vector3 *a, vector3 *b);
void v3Multiply(vector3 *inOut, float s);
void v3MultiplyToR(vector3 *result, vector3 *v, float c);
void v3Divide(vector3 *inOut, float s);
void v3DivideToR(vector3 *result, vector3 *v, float s);
float v3SquaredMagnitude(vector3 *v);
float v3Magnitude(vector3 *v);
float v3DotProduct(vector3 *a, vector3 *b);
void v3Normalize(vector3 *inOut);
void v3CrossProduct(vector3 *result, vector3 *a, vector3 *b);
void v3AxisToQuaternion(quaternion *result, vector3 *v, float phi);
bool v3IsEqual(vector3 *a, vector3 *b);
//void v3AngularVelocityToQuaternion(quaternion *inOut, vector3 *vIn);

// quaternion
void qSetZero(quaternion *q);
void qSet(quaternion *result, float x, float y, float z, float w);
void qCopy(quaternion *inOut, quaternion *q);
void qSetIdentity(quaternion *q);
void qAdd(quaternion *inOut, quaternion *q);
void qScale(quaternion *inOut, float s);
void qMultiply(quaternion *inOut, quaternion *q);
void qMultiplyToR(quaternion *result, quaternion *a, quaternion *b);
void qNormalize(quaternion *q);
void qConvertToMatrix(matrix44 *inOut, quaternion *q);

// matrix 4x4
matrix44 mSetPerspectiveBasic(float right, float top, float near, float far);
matrix44 mSetPerspective(float fovyRadians, float aspect, float nearZ, float farZ);
void mCopy(matrix44 *a, matrix44 *b);
void mSetIdentity(matrix44 *m);
//void mSetTranslation(matrix44 *m, vector3* v);
void mSetTranslation(matrix44 *m, float x, float y, float z);
void mSetRotation(matrix44* m, float angle, float x, float y, float z);
void mMultiplyToR(matrix44 *left, matrix44 *right, matrix44 *result);
void mMultiply(matrix44 *left, matrix44 *right);
matrix44 mSetOrthographic(float left, float right, float bottom, float top, float near, float far);
void mTranspose(matrix44 *m);
float mDeterminant(matrix44 *m);
void mScale(matrix44 *m, float s);
void mInvert(matrix44 *m);
void mInvertToR(matrix44 *m, matrix44 *result);
void mMultiplyVector(matrix44 *m, vector3 *v);


// vector 2D implementation

inline void v2SetZero(vector2 *v)
{
    v->x = 0.0f;
    v->y = 0.0f;
}

inline void v2Copy(vector2 *inOut, vector2 *v)
{
    inOut->x = v->x;
    inOut->y = v->y;
}

inline void v2Set(vector2 *inOut, float x, float y)
{
    inOut->x = x;
    inOut->y = y;
}

inline void v2PerpendicularCCW(vector2 *v)
{
    float temp = -v->y;
    
    v->y = v->x;
    v->x = temp;
}

inline void v2PerpendicularCW(vector2 *v)
{
    float temp = -v->x;
    
    v->x = v->y;
    v->y = temp;
}

// vector 3D implementation

inline void v3SetZero(vector3 *v)
{
    v->x = 0;
    v->y = 0;
    v->z = 0;
}

inline void v3Copy(vector3 *inOut, vector3 *v)
{
    inOut->x = v->x;
    inOut->y = v->y;
    inOut->z = v->z;
}

inline void v3Set(vector3 *result, float x, float y, float z)
{
    result->x = x;
    result->y = y;
    result->z = z;
}

inline void v3Add(vector3 *inOut, vector3 *v)
{
    inOut->x += v->x;
    inOut->y += v->y;
    inOut->z += v->z;
}

inline void v3AddToR(vector3 *result, vector3 *a, vector3 *b)
{
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

inline void v3Subtract(vector3 *inOut, vector3 *v)
{
    inOut->x -= v->x;
    inOut->y -= v->y;
    inOut->z -= v->z;
}

inline void v3SubtractToR(vector3 *result, vector3 *a, vector3 *b)
{
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

inline void v3Multiply(vector3 *inOut, float s)
{
    inOut->x *= s;
    inOut->y *= s;
    inOut->z *= s;
}

inline void v3MultiplyToR(vector3 *result, vector3 *v, float s)
{
    result->x = v->x * s;
    result->y = v->y * s;
    result->z = v->z * s;
}

inline void v3Divide(vector3 *inOut, float s)
{
    inOut->x /= s;
    inOut->y /= s;
    inOut->z /= s;
}

inline void v3DivideToR(vector3 *result, vector3 *v, float s)
{
    result->x = v->x / s;
    result->y = v->y / s;
    result->z = v->z / s;
}

inline float v3SquaredMagnitude(vector3 *v)
{
    return ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

inline float v3Magnitude(vector3 *v)
{
    return fastSqrt(v3SquaredMagnitude(v));
}

inline float v3DotProduct(vector3 *a, vector3 *b)
{
    return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z));
}

inline void v3Normalize(vector3 *v)
{
    float c = v3Magnitude(v);
    
    if (c != 0.0f)
    {
        v->x /= c;
        v->y /= c;
        v->z /= c;
    }
}

inline void v3CrossProduct(vector3 *result, vector3 *a, vector3 *b)
{
    result->x = (a->y * b->z) - (a->z * b->y);
    result->y = (a->z * b->x) - (a->x * b->z);
    result->z = (a->x * b->y) - (a->y * b->x);
}

// converting a 3d vector to a quaternion
inline void v3AxisToQuaternion(quaternion *inOut, vector3 *v, float phi)
{
    float halfAngle = phi * 0.5f;
    float alpha = sinf(halfAngle);
    
    inOut->x = v->x * alpha;
    inOut->y = v->y * alpha;
    inOut->z = v->z * alpha;
    inOut->w = cosf(halfAngle);
}

inline bool v3IsEqual(vector3 *a, vector3 *b)
{
    return (bool)memcmp(a, b, sizeof(vector3)) == 0;
}

//inline void v3AngularVelocityToQuaternion(quaternion *qOut, vector3 *vIn)
//{
//    qOut->x = vIn->x;
//    qOut->y = vIn->y;
//    qOut->z = vIn->z;
//    qOut->w = 0.0f;
//}

// quaternion implementation

inline void qSetZero(quaternion *q)
{
    q->x = 0.0f;
    q->y = 0.0f;
    q->z = 0.0f;
    q->w = 0.0f;
}

inline void qSet(quaternion *result, float x, float y, float z, float w)
{
    result->x = x;
    result->y = y;
    result->z = z;
    result->w = w;
}

inline void qCopy(quaternion *inOut, quaternion *q)
{
    inOut->x = q->x;
    inOut->y = q->y;
    inOut->z = q->z;
    inOut->w = q->w;
}

inline void qSetIdentity(quaternion *q)
{
    q->x = 0.0f;
    q->y = 0.0f;
    q->z = 0.0f;
    q->w = 1.0f;
}

inline void qAdd(quaternion *inOut, quaternion* q)
{
    inOut->x += q->x;
    inOut->y += q->y;
    inOut->z += q->z;
    inOut->w += q->w;
}

inline void qScale(quaternion *inOut, float s)
{
    inOut->x *= s;
    inOut->y *= s;
    inOut->z *= s;
    inOut->w *= s;
}

inline void qMultiply(quaternion *inOut, quaternion *q)
{
    quaternion r;
    
    r.x = inOut->w * q->x + inOut->x * q->w + inOut->y * q->z - inOut->z * q->y;
    r.y = inOut->w * q->y - inOut->x * q->z + inOut->y * q->w + inOut->z * q->x;
    r.z = inOut->w * q->z + inOut->x * q->y - inOut->y * q->x + inOut->z * q->w;
    r.w = inOut->w * q->w - inOut->x * q->x - inOut->y * q->y - inOut->z * q->z;
    
    qCopy(inOut, &r);
}

inline void qMultiplyToR(quaternion *result, quaternion *a, quaternion *b)
{
    result->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    result->y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    result->z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
    result->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
}

inline void qNormalize(quaternion *q)
{
    float m = fastSqrt((q->x * q->x) + (q->y * q->y)
                         + (q->z * q->z) + (q->w * q->w));
    
    if (m > 0.0f)
    {
        q->x /= m;
        q->y /= m;
        q->z /= m;
        q->w /= m;
    }
}

inline void qConvertToMatrix (matrix44 *inOut, quaternion *q)
{
    inOut->m[0] = 1.0f - 2.0f * (q->z * q->z + q->y * q->y);
    inOut->m[5] = 1.0f - 2.0f * (q->z * q->z + q->x * q->x);
    inOut->m[10] = 1.0f - 2.0f * (q->y * q->y + q->x * q->x);
    
    inOut->m[1] = 2.0f * (q->x * q->y - q->z * q->w);
    inOut->m[2] = 2.0f * (q->x * q->z + q->y * q->w);
    
    inOut->m[4] = 2.0f * (q->x * q->y + q->w * q->z);
    inOut->m[6] = 2.0f * (q->z * q->y - q->x * q->w);
    
    inOut->m[8] = 2.0f * (q->x * q->z - q->w * q->y);
    inOut->m[9] = 2.0f * (q->y * q->z + q->w * q->x);
    
    inOut->m[12] = 0.0f;
    inOut->m[13] = 0.0f;
    inOut->m[14] = 0.0f;
    inOut->m[3] = 0.0f;
    inOut->m[7] = 0.0f;
    inOut->m[11] = 0.0f;
    inOut->m[15] = 1.0f;
}

// matrix4x4 implementation

inline void mCopy(matrix44 *a, matrix44 *b)
{
    memcpy(a, b, sizeof(matrix44));
}

inline void mSetIdentity(matrix44 *m)
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

inline matrix44 mSetPerspective(float fovyRadians, float aspect, float nearZ, float farZ)
{
    float cotan = 1.0f / tanf(fovyRadians * 0.5f);
    
    matrix44 m = { cotan / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, cotan, 0.0f, 0.0f,
        0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), -1.0f,
        0.0f, 0.0f, (2.0f * farZ * nearZ) / (nearZ - farZ), 0.0f };
    
    return m;
}

inline matrix44 mSetPerspectiveBasic(float right, float top, float near, float far)
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

inline matrix44 mSetOrthographic(float left, float right, float bottom, float top, float near, float far)
{
    float ral = right + left;
    float rsl = right - left;
    float tab = top + bottom;
    float tsb = top - bottom;
    float fan = far + near;
    float fsn = far - near;
    
    matrix44 m = { 2.0f / rsl, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / tsb, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / fsn, 0.0f,
        -ral / rsl, -tab / tsb, -fan / fsn, 1.0f };
    
    return m;
}

inline void mSetTranslation(matrix44 *m, float x, float y, float z)
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

//inline void mSetTranslation(matrix44 *m, vector3* v)
//{
//    m->m[12] = v->x;
//    m->m[13] = v->y;
//    m->m[14] = v->z;
//}

inline void mSetScale(matrix44* m, float x, float y, float z)
{
    m->m[0] = x;
    m->m[5] = y;
    m->m[10] = z;
}

inline void mSetRotation(matrix44* m, float angle, float x, float y, float z)
{
    float length = fastSqrt(x * x + y * y + z * z);
    float xp = x / length;
    float yp = y / length;
    float zp = z / length;
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

inline void mMultiplyToR(matrix44 *left, matrix44 *right, matrix44 *result)
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

inline void mMultiply(matrix44 *left, matrix44 *right)
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

inline void mScale(matrix44 *m, float s)
{
    int i;
    
    for(i = 0; i < 16; i++)
        m->m[i] *= s;
}

inline void mInvert(matrix44 *m)
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

inline void mInvertToR(matrix44 *m, matrix44 *result)
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

inline float mDeterminant(matrix44 *m)
{
    return ((m->m03 * m->m12 * m->m21 * m->m30) - (m->m02 * m->m13 * m->m21 * m->m30) - (m->m03 * m->m11 * m->m22 * m->m30) + (m->m01 * m->m13 * m->m22 * m->m30) + (m->m02 * m->m11 * m->m23 * m->m30) - (m->m01 * m->m12 * m->m23 * m->m30) - (m->m03 * m->m12 * m->m20 * m->m31) + (m->m02 * m->m13 * m->m20 * m->m31) + (m->m03 * m->m10 * m->m22 * m->m31) - (m->m00 * m->m13 * m->m22 * m->m31) - (m->m02 * m->m10 * m->m23 * m->m31) + (m->m00 * m->m12 * m->m23 * m->m31) + (m->m03 * m->m11 * m->m20 * m->m32) - (m->m01 * m->m13 * m->m20 * m->m32) - (m->m03 * m->m10 * m->m21 * m->m32) + (m->m00 * m->m13 * m->m21 * m->m32) + (m->m01 * m->m10 * m->m23 * m->m32) - (m->m00 * m->m11 * m->m23 * m->m32) - (m->m02 * m->m11 * m->m20 * m->m33) + (m->m01 * m->m12 * m->m20 * m->m33) + (m->m02 * m->m10 * m->m21 * m->m33) - (m->m00 * m->m12 * m->m21 * m->m33) - (m->m01 * m->m10 * m->m22 * m->m33) + (m->m00 * m->m11 * m->m22 * m->m33));
}



inline void mMultiplyVector(matrix44 *m, vector3 *v)
{
    vector3	temp;
    float r = m->m30 * v->x + m->m31 * v->y + m->m32 * v->z + m->m33;

    temp.x = (m->m00 * v->x + m->m01 * v->y + m->m02 * v->z + m->m03) / r;
    temp.y = (m->m10 * v->x + m->m11 * v->y + m->m12 * v->z + m->m13) / r;
    temp.z = (m->m20 * v->x + m->m21 * v->y + m->m22 * v->z + m->m23) / r;
    
    memcpy(v, &temp, sizeof(vector3));
}

inline void mTranspose(matrix44 *m)
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

#endif
