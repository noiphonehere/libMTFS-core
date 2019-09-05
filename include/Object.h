/*
 Object.h

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

#ifndef OBJECT_H
#define OBJECT_H
#include <wavefront.h>


struct Vector3D
{
	union { float x; float pitch; };
	union { float y; float yaw; };
	union { float z; float roll; };
	
};

class Object
{
protected:
	OBJFile model;
	Vector3D position;
	Vector3D orientation;
	Vector3D velocity;
	Vector3D scale;
	unsigned int displayList;
	int dont_use_display_lists, dont_use_arrays;
public:
	Object();
	Object(const char *modelFileName);
	~Object();
	virtual void loadModel(const char *modelFileName);
	virtual void Draw();
	virtual void setPosition(float x, float y, float z);
	virtual void setOrientation(float newPitch, float newRoll, float newYaw);
	virtual void setVelocity(float x, float y, float z);
	virtual void setScale(float x, float y, float z);
	virtual Vector3D *getPosition() {return &position;}
	virtual Vector3D *getOrientation() {return &orientation;}
	virtual Vector3D *getVelocity() {return &velocity;}
	virtual Vector3D *getScale() {return &scale;}
	virtual void update(float delta_time);
	virtual void cleanupGL();
};
#endif
