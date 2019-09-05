/*
 Camera.cpp
 Created by Carlo Retta on 9/9/11.

 Copyright (c) 2011-2012 Carlo Retta.

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
#ifndef USE_ES
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#else
#include <GLES/gl.h>
#endif

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
#define WITHIN_THRESHOLD(ratio) > ((distance/RATE_DIVISOR)*delta_time*ratio/7.0f)

void Camera::update(float delta_time)
{
	if(!isMoving) return;		//Don't do processing we don't have to do
	
	float speed_mult = (distance-recalcDistance())/distance;	//Fraction of distance we have left to cover
	
	speed_mult = powf(
			sinf( speed_mult*( (4.0f*M_PI)/5.0f ) + M_PI/8.0f) + delta_time,
			 1.5f);	//Smooth movement rate
	
	if(pos.x != dest.x && fabs(pos.x-dest.x) WITHIN_THRESHOLD(xR))
	{
		pos.x -= (distance/RATE_DIVISOR)*(fabs(pos.x-dest.x)/(pos.x-dest.x))*delta_time*speed_mult*xR;
	}
	
	if(pos.y != dest.y && fabs(pos.y-dest.y) WITHIN_THRESHOLD(yR))
	{
		pos.y -= (distance/RATE_DIVISOR)*(fabs(pos.y-dest.y)/(pos.y-dest.y))*delta_time*speed_mult*yR;
	}
	
	if(pos.z != dest.z && fabs(pos.z-dest.z) WITHIN_THRESHOLD(zR))
	{
		pos.z -= (distance/RATE_DIVISOR)*(fabs(pos.z-dest.z)/(pos.z-dest.z))*delta_time*speed_mult*zR;
	}
	
	
	if( !(recalcDistance() WITHIN_THRESHOLD(1)) )
	{
		memcpy(&pos, &dest, sizeof(Vector3D));
		
		if(destCallback) destCallback(cbData); //Do whatever we're supposed to do when we get there
		destCallback = 0;	//Set the callback pointer to zero
		
		isMoving = false;	//We're not moving anymore
	}
	
	//***********Now do the rotation**************//
	updateRotation(delta_time);
}

float Camera::recalcDistance()
{
	#if defined(POWERPC)
	
	vector float d = {dest.x, dest.y, dest.z};	//Load vectors for Altivec
	vector float p = {pos.x, pos.y, pos.z};
	static vector float zero = {0,0,0};
	
	vector float dist = vec_abs(vec_sub(d,p));	//Calculate distance
	vector float dist2 = vec_madd(dist, dist, zero);	//Square it
	
	float *pfdist2 = (float*)&dist2; //Get an array pointer to the vector
	return sqrtf(pfdist2[0]+pfdist2[1]+pfdist2[2]);	//Return the distance

	#elif defined(I386) || defined(X86_64)
	vfloat4 d = {dest.x, dest.y, dest.z, 0.0f};	//Load vectors for SSE
	vfloat4 p = {pos.x, pos.y, pos.z, 0.0f};
	
	vfloat4 dist;
	dist.v = d.v-p.v;
	dist.v *= dist.v;
	return sqrtf(dist.s.x + dist.s.y + dist.s.z);
	#else		//Use plain C++ version for the boring processors. TODO: Add ARM NEON support
	
	float distX = fabs(dest.x-pos.x);
	float distY = fabs(dest.y-pos.y);
	float distZ = fabs(dest.z-pos.z);

	return sqrtf(distX*distX + distY*distY + distZ*distZ);
	
	#endif
}

void Camera::setPosition(float newX, float newY, float newZ, int destination)
{
	if(destination)
	{
		isMoving = true;	//Now we are moving
		
		dest.x = newX;
		dest.y = newY;
		dest.z = newZ;
		
		distance = recalcDistance();
		xR = fabs(pos.x-dest.x)/distance;
		yR = fabs(pos.y-dest.y)/distance;
		zR = fabs(pos.z-dest.z)/distance;
		
	}
	else
	{
		pos.x = newX;
		pos.y = newY;
		pos.z = newZ;
		
		memcpy(&dest, &pos, sizeof(Vector3D));	//Make sure the destination is the same as our position
	}
}


void Camera::setPosition(Vector3D *newPos, int destination)
{
	if(destination)
	{
		isMoving = true;
		
		memcpy(&dest, newPos, sizeof(Vector3D));
		
		distance = recalcDistance();
		xR = fabs(pos.x-dest.x)/distance;
		yR = fabs(pos.y-dest.y)/distance;
		zR = fabs(pos.z-dest.z)/distance;
	}
	else
	{
		memcpy(&pos, newPos, sizeof(Vector3D));
		memcpy(&dest, newPos, sizeof(Vector3D));
	}
}

void Camera::setLightPos(float pos[4])
{
	memcpy(lightPos, pos, 4*sizeof(float));
}

void Camera::translateCamera()
{
	glRotatef(-rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(-rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(-rot.z, 0.0f, 0.0f, 1.0f);
	glTranslatef(-pos.x, -pos.y, -pos.z);
	
	/*float newLightPos[4];
	memcpy(newLightPos, lightPos, 4*sizeof(float));
	newLightPos[0] += pos.x;
	newLightPos[1] += pos.y;
	newLightPos[2] += pos.z;
	glLightfv(GL_LIGHT0, GL_POSITION, newLightPos);*/
}

void Camera::setCallback(void (*cb)(void*), void *data)
{
	destCallback = cb;
	cbData = data;
}
