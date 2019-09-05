/*
 Camera.cpp

 Copyright (c) 2012 Carlo Retta.

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/

#include "Camera.h"
#include <math.h>
#include <memory.h>
#if defined(POWERPC)
#include <altivec.h>
#elif defined(I386) || defined(X86_64)
typedef float v4sf __attribute__((vector_size (16)));
union vfloat4
{
	v4sf v;
	struct { float x, y, z, w; } s;
};
#endif

#define RATE_DIVISOR 1.5f
#define WITHIN_RTHRESHOLD(ratio) > ((rdistance/RATE_DIVISOR)*delta_time*ratio/2.0f)

void Camera::updateRotation(float delta_time)
{
	float rspeed_mult = (rdistance-recalcRDistance())/rdistance;
	
	rspeed_mult = powf(
			sinf( rspeed_mult * (4.0f*M_PI/5.0f) + M_PI/8.0f ) + delta_time,
				 1.5f);	//Smooth movement rate

	if(rot.x != rdest.x && fabs(rot.x-rdest.x) WITHIN_RTHRESHOLD(rxR))
	{
		rot.x -= (rdistance/RATE_DIVISOR)*(fabs(rot.x-rdest.x)/(rot.x-rdest.x))*delta_time*rspeed_mult*rxR;
	}
	
	if(rot.y != rdest.y && fabs(rot.y-rdest.y) WITHIN_RTHRESHOLD(ryR))
	{
		rot.y -= (rdistance/RATE_DIVISOR)*(fabs(rot.y-rdest.y)/(rot.y-rdest.y))*delta_time*rspeed_mult*ryR;
	}
	
	if(rot.z != rdest.z && fabs(rot.z-rdest.z) WITHIN_RTHRESHOLD(rzR))
	{
		rot.z -= (rdistance/RATE_DIVISOR)*(fabs(rot.z-rdest.z)/(rot.z-rdest.z))*delta_time*rspeed_mult*rzR;
	}
	
	
	if(!(recalcRDistance() WITHIN_RTHRESHOLD(1))) memcpy(&rot, &rdest, sizeof(Vector3D));
}
float Camera::recalcRDistance()
{
	#if defined(POWERPC)
	
	vector float d = {rdest.x, rdest.y, rdest.z};	//Load vectors for Altivec
	vector float p = {rot.x, rot.y, rot.z};
	static vector float zero = {0,0,0};
	
	vector float dist = vec_abs(vec_sub(d,p));	//Calculate distance
	vector float dist2 = vec_madd(dist, dist, zero);	//Square it
	
	float *pfdist2 = (float*)&dist2; //Get an array pointer to the vector
	return sqrtf(pfdist2[0]+pfdist2[1]+pfdist2[2]);	//Return the distance

	#elif defined(I386) || defined(X86_64)
	vfloat4 d = {rdest.x, rdest.y, rdest.z, 0.0f};	//Load vectors for SSE
	vfloat4 p = {rot.x, rot.y, rot.z, 0.0f};
	
	vfloat4 dist;
	dist.v = d.v-p.v;
	dist.v *= dist.v;
	return sqrtf(dist.s.x + dist.s.y + dist.s.z);
	#else		//Use plain C++ version for the boring processors. TODO: Add ARM NEON support
	
	float distX = fabs(rdest.x-rot.x);
	float distY = fabs(rdest.y-rot.y);
	float distZ = fabs(rdest.z-rot.z);

	return sqrtf(distX*distX + distY*distY + distZ*distZ);
	
	#endif
}

void Camera::setRotation(float newX, float newY, float newZ, int destination)
{
	if(destination)
	{
		rdest.x = newX;
		rdest.y = newY;
		rdest.z = newZ;
		
		rdistance = recalcRDistance();
		rxR = fabs(rot.x-rdest.x)/rdistance;
		ryR = fabs(rot.y-rdest.y)/rdistance;
		rzR = fabs(rot.z-rdest.z)/rdistance;
	}
	else
	{
		rot.x = newX;
		rot.y = newY;
		rot.z = newZ;
		
		memcpy(&rdest, &rot, sizeof(Vector3D));
	}
	
}

void Camera::setRotation(Vector3D *newPos, int destination)
{
	if(destination)
	{
		memcpy(&rdest, newPos, sizeof(Vector3D));
		
		rdistance = recalcRDistance();
		rxR = fabs(rot.x-rdest.x)/rdistance;
		ryR = fabs(rot.y-rdest.y)/rdistance;
		rzR = fabs(rot.z-rdest.z)/rdistance;
	}
	else
	{
		memcpy(&rot, newPos, sizeof(Vector3D));
		memcpy(&rdest, newPos, sizeof(Vector3D));
	}
}
