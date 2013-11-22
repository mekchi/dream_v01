
/* mekchi */

#ifndef _MEKCHI_VECTOR_
#define _MEKCHI_VECTOR_

#include <math.h>
#include <string.h>

#include "FastMath.h"
#include "Quaternion.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
typedef union
{
    struct
    {
        float_t x;
        float_t y;
    };
    float_t v[2];
} vector2;
    
typedef union
{
    struct
    {
        float_t x;
        float_t y;
        float_t z;
    };
    float_t v[3];
} vector3;

static inline void vSetZero(vector3 *v);
static inline void vCopy(vector3 *a, vector3 *b);
static inline void vSet(vector3 *v, float_t x, float_t y, float_t z);
static inline void vAdd(vector3 *a, vector3 *b);
static inline void vAddToR(vector3 *r, vector3 *a, vector3 *b);
static inline void vSubtract(vector3 *a, vector3 *b);
static inline void vSubtractToR(vector3 *r, vector3 *a, vector3 *b);
static inline void vMultiply(vector3 *v, float_t c);
static inline void vMultiplyToR(vector3 *r, vector3 *v, float_t c);
static inline void vDivide(vector3 *v, float_t c);
static inline void vDivideToR(vector3 *r, vector3 *v, float_t c);
static inline float_t vSquaredMagnitude(vector3 *v);
static inline float_t vMagnitude(vector3 *v);
static inline float_t vDotProduct(vector3 *a, vector3 *b);
static inline void vNormalize(vector3 *v);
static inline void vCrossProduct(vector3 *r, vector3 *a, vector3 *b);
static inline void vAxisToQuaternion(quaternion *q, vector3 *v, float_t phi);
static inline int vIsEqual(vector3 *a, vector3 *b);
static inline void vAngularVelocityToQuaternion(quaternion *qOut, vector3 *vIn);

        
// implementation
    
static inline void vSetZero(vector3 *v)
{
    v->x = 0;
    v->y = 0;
    v->z = 0;
}

static inline void vCopy(vector3 *a, vector3 *b)
{
    a->x = b->x;
    a->y = b->y;
    a->z = b->z;
}

static inline void vSet(vector3 *v, float_t x, float_t y, float_t z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

static inline void vAdd(vector3 *a, vector3 *b)
{
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

static inline void vAddToR(vector3 *r, vector3 *a, vector3 *b)
{
    r->x = a->x + b->x;
    r->y = a->y + b->y;
    r->z = a->z + b->z;
}

static inline void vSubtract(vector3 *a, vector3 *b)
{
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}

static inline void vSubtracToR(vector3 *r, vector3 *a, vector3 *b)
{
    r->x = a->x - b->x;
    r->y = a->y - b->y;
    r->z = a->z - b->z;
}

static inline void vMultiply(vector3 *v, float_t c)
{
    v->x *= c;
    v->y *= c;
    v->z *= c;
}

static inline void vMultiplyToR(vector3 *r, vector3 *v, float_t c)
{
    r->x = v->x * c;
    r->y = v->y * c;
    r->z = v->z * c;
}

static inline void vDivide(vector3 *v, float_t c)
{
    v->x /= c;
    v->y /= c;
    v->z /= c;
}

static inline void vDivideToR(vector3 *r, vector3 *v, float_t c)
{
    r->x = v->x / c;
    r->y = v->y / c;
    r->z = v->z / c;
}

static inline float_t vSquaredMagnitude(vector3 *v)
{
    return ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

static inline float_t vMagnitude(vector3 *v)
{    
    return fastSqrt(vSquaredMagnitude(v));
}

static inline float_t vDotProduct(vector3 *a, vector3 *b)
{
    return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z));
}

static inline void vNormalize(vector3 *v)
{
    float_t c = vMagnitude(v);

    if (c != 0.0f)
    {
        v->x /= c;
        v->y /= c;
        v->z /= c;
    }
}

static inline void vCrossProduct(vector3 *r, vector3 *a, vector3 *b)
{
    r->x = (a->y * b->z) - (a->z * b->y);
    r->y = (a->z * b->x) - (a->x * b->z);
    r->z = (a->x * b->y) - (a->y * b->x);
}

// converting a 3d vector to a quaternion
static inline void vAxisToQuaternion(quaternion *q, vector3 *v, float_t phi)
{
    float_t halfAngle = phi * 0.5f;
    float_t alpha = sinf(halfAngle);
    
    q->x = v->x * alpha;
    q->y = v->y * alpha;
    q->z = v->z * alpha;
    q->w = cosf(halfAngle);
}
    
static inline int vIsEqual(vector3 *a, vector3 *b)
{
    return memcmp(a, b, sizeof(vector3)) == 0;
}

static inline int vIsGreater(vector3 *a, vector3 *b)
{
    return (a->x > b->x || a->y > b->y || a->z > b->z);
}
    
static inline void vAngularVelocityToQuaternion(quaternion *qOut, vector3 *vIn)
{
    qOut->x = vIn->x;
    qOut->y = vIn->y;
    qOut->z = vIn->z;
    qOut->w = 0.0f;
}

#ifdef __cplusplus
}
#endif

#endif
