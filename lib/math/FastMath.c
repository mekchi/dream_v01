
/* mekchi */

#include "FastMath.h"

#include <math.h>

float inverseSqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = number * 0.5f;
    y  = number;
    i  = *(long *)&y;  // evil floating point bit level hacking
    i  = 0x5f3759df - (i >> 1); // what the fuck?
    y  = *(float *)&i;
    y  = y * (threehalfs - ( x2 * y * y)); // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

    return y;
}

float fastSqrt(float number)
{
    return sqrtf(number);

//    union
//    {
//        int tmp;
//        float val;
//    } u;
//
//    u.val = number;
//    u.tmp -= 1<<23; /* Remove last bit so 1.0 gives 1.0 */
//    /* tmp is now an approximation to logbase2(val) */
//    u.tmp >>= 1; /* divide by 2 */
//    u.tmp += 1<<29; /* add 64 to exponent: (e+127)/2 =(e/2)+63, */
//    /* that represents (e/2)-64 but want e/2 */
//    return u.val;
}

float fastLog2(float n)
{
    static const float LogBodge = 0.346607f;
    static const float OOshift23 = 1.0f / (1<<23);

    float x;
    float y;

    x =* (int*)&n;
    x *= OOshift23; //1/pow(2,23);
    x = x - 127;
    y = x - floorf(x);
    y = (y - y * y) * LogBodge;

    return x + y;
}

float fastPow2(float n)
{
    static const float shift23 = 1 << 23;
    static const float PowBodge = 0.33971f;

    float x;
    float y = n - floorf(n);
    y = (y - y * y) * PowBodge;

    x = n + 127.0f - y;
    x *= shift23; //pow(2,23);
    *(int*)&x = (int)x;

    return x;
}

float fastPow(float a, float b)
{
    return fastPow2(b * fastLog2(a));
}

float fastAbs(float number)
{
    return number < 0.0f ? -number : number;
}
