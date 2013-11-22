
/* mekchi */

#ifndef _MEKCHI_QUATERNION_
#define _MEKCHI_QUATERNION_

#include <string.h>

#include "FastMath.h"
#include "Matrix.h"

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
        float_t z;
        float_t w;
    };
    float_t q[4];
} quaternion;
    

static inline void qSetZero(quaternion *q);
static inline void qSet(quaternion *q, float x, float y, float z, float w);
static inline void qCopy(quaternion *a, quaternion *b);
static inline void qSetIdentity(quaternion *q);
static inline void qAdd(quaternion *qAInOut, quaternion* qBIn);
static inline void qScale(quaternion *qInOut, float scale);
static inline void qMultiply(quaternion *a, quaternion *b);
static inline void qMultiplyToR(quaternion *r, quaternion *a, quaternion *b);
static inline void qNormalize(quaternion *q);
static inline void qConvertToMatrix(matrix44 *m, quaternion *q);
    
// implementation
    
static inline void qSetZero(quaternion *q)
{
    q->x = 0.0f;
    q->y = 0.0f;
    q->z = 0.0f;
    q->w = 0.0f;
}
    
static inline void qSet(quaternion *q, float x, float y, float z, float w)
{
    q->x = x;
    q->y = y;
    q->z = z;
    q->w = w;
}

static inline void qCopy(quaternion *a, quaternion *b)
{
    a->x = b->x;
    a->y = b->y;
    a->z = b->z;
    a->w = b->w;
}

static inline void qSetIdentity(quaternion *q)
{
    q->x = 0.0f;
    q->y = 0.0f;
    q->z = 0.0f;
    q->w = 1.0f;
}
    
static inline void qAdd(quaternion *qAInOut, quaternion* qBIn)
{
    qAInOut->x += qBIn->x;
    qAInOut->y += qBIn->y;
    qAInOut->z += qBIn->z;
    qAInOut->w += qBIn->w;
}
 
static inline void qScale(quaternion *qInOut, float scale)
{
    qInOut->x *= scale;
    qInOut->y *= scale;
    qInOut->z *= scale;
    qInOut->w *= scale;
}

static inline void qMultiply(quaternion *a, quaternion *b)
{
    quaternion r;
    
    r.x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    r.y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    r.z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
    r.w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
    
    qCopy(a, &r);
}

static inline void qMultiplyToR(quaternion *r, quaternion *a, quaternion *b)
{
    r->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    r->y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    r->z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
    r->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
}

static inline void qNormalize(quaternion *q)
{
    float_t m = fastSqrt((q->x * q->x) + (q->y * q->y)
                       + (q->z * q->z) + (q->w * q->w));
    
    if (m > 0.0f)
    {
        q->x /= m;
        q->y /= m;
        q->z /= m;
        q->w /= m;
    }
}

static inline void qConvertToMatrix (matrix44 *m, quaternion *q)
{
    m->m[0] = 1.0f - 2.0f * (q->z * q->z + q->y * q->y);
    m->m[5] = 1.0f - 2.0f * (q->z * q->z + q->x * q->x);
    m->m[10] = 1.0f - 2.0f * (q->y * q->y + q->x * q->x);
    
    m->m[1] = 2.0f * (q->x * q->y - q->z * q->w);
    m->m[2] = 2.0f * (q->x * q->z + q->y * q->w);
    
    m->m[4] = 2.0f * (q->x * q->y + q->w * q->z);
    m->m[6] = 2.0f * (q->z * q->y - q->x * q->w);
    
    m->m[8] = 2.0f * (q->x * q->z - q->w * q->y);
    m->m[9] = 2.0f * (q->y * q->z + q->w * q->x);
    
    m->m[12] = 0.0f;
    m->m[13] = 0.0f;
    m->m[14] = 0.0f;
    m->m[3] = 0.0f;
    m->m[7] = 0.0f;
    m->m[11] = 0.0f;
    m->m[15] = 1.0f;
}
    

#ifdef __cplusplus
}
#endif

#endif

