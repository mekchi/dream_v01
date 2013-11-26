
/* mekchi */

#ifndef _MEKCHI_FM_
#define _MEKCHI_FM_

#ifdef __cplusplus
extern "C"
{
#endif
    
#define PI  3.14159265f
#define DEGREE_TO_RADIAN(x) ((x) * 0.017453292f)

float inverseSqrt(float number);
float fastSqrt(float number);
float fastLog2(float n);
float fastPow2(float n);
float fastPow(float a, float b);
float fastAbs(float number);

#ifdef __cplusplus
}
#endif

#endif