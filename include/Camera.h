/*
 Camera.h
 Created by Carlo Retta on 9/9/2011.

Copyright (c) 2011-2012 Carlo Retta

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

#ifndef CAMERA_H
#define CAMERA_H
#include <Object.h>	//For Vector3D struct

class Camera
{
	Vector3D pos, dest, dist;
	Vector3D rot, rdest, rdist;
	float distance, rdistance;		//So that we can keep track of rate of movement
	float xR, yR, zR, rxR, ryR, rzR;
	
	float lightPos[4];
	
	void (*destCallback)(void *);
	void *cbData;
	
	bool isMoving;
	
	protected:
	float recalcDistance();
	float recalcRDistance();
	
	public:
	void update(float delta_time);
	void updateRotation(float delta_time);
	
	void setPosition(float newX, float newY, float newZ, int destination = 1);
	void setPosition(Vector3D *newPos, int destination = 1);
	void setLightPos(float pos[4]);
	
	void setRotation(float newX, float newY, float newZ, int destination = 1);
	void setRotation(Vector3D *newPos, int destination = 1);
	
	Vector3D *getPosition(int destination = 0);
	
	void translateCamera();
	
	void setCallback(void (*cb)(void*), void *data);
};

#endif
