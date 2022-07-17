#ifndef __VECMATH_H
#define __VECMATH_H
#include <math.h>

inline float dot(float v1[], float v2[])
{
     return (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]);
}

inline void CROSS(float res[], float v1[], float v2[])
{
     res[0] = v1[1] * v2[2] - v1[2]*v2[1];
     res[1] = v2[0] * v1[2] - v1[0]*v2[2];
     res[2] = v1[0] * v2[1] - v2[0]*v1[1];
}

inline float MAGN(float v[])
{
     return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

inline void NORM(float v[])
{
     float magn = MAGN(v);
     if (magn>0.f)
     {
	  v[0]/=magn;
	  v[1]/=magn;
	  v[2]/=magn;
     }
}

inline void SUB(float res[], float v1[], float v2[])
{
     res[0]=v1[0]-v2[0];
     res[1]=v1[1]-v2[1];
     res[2]=v1[2]-v2[2];
}

inline void ADD(float res[], float v1[], float v2[])
{
     res[0]=v1[0]+v2[0];
     res[1]=v1[1]+v2[1];
     res[2]=v1[2]+v2[2];
}

void triNormal(float v1[], float v2[], float v3[], float res[])
{
     float a[3], b[3], c[3];

     SUB(a,v1, v3);
     SUB(b,v2, v3);

     CROSS(res, b, a);
//     NORM(res);
}

#endif
