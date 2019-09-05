/*
 Object.cpp
 Object class implementation file

 Copyright (C) 2011-2012, 2019 Carlo Retta

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

#include "Object.h"

#ifndef USE_ES
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#else
#include <GLES/gl.h>
#endif

#include <wfdisplay.h>

Object::Object()
{
	scale.x = scale.y = scale.z = 1.0f;
	orientation.roll = orientation.pitch = orientation.yaw = 0.0f;
	position.x = position.y = position.z = 0.0f;
	
	displayList = 0;
	dont_use_display_lists = dont_use_arrays = 0;
}

Object::Object(const char *modelFileName)
{
	loadModel(modelFileName);
	
	scale.x = scale.y = scale.z = 1.0f;
	orientation.roll = orientation.pitch = orientation.yaw = 0.0f;
	position.x = position.y = position.z = 0.0f;
	
	displayList = 0;
	dont_use_display_lists = dont_use_arrays = 0;
}

Object::~Object()
{
	
}

void Object::loadModel(const char *modelFileName)
{
	model.loadModel(modelFileName);
}

void Object::Draw()
{
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotatef(orientation.roll, 0.0f, 0.0f, 1.0f);
		glRotatef(orientation.pitch, 1.0f, 0.0f, 0.0f);
		glRotatef(orientation.yaw, 0.0f, 1.0f, 0.0f);
		glScalef(scale.x, scale.y, scale.z);
		
		#ifndef NO_DISPLAY_LISTS
		if(displayList) glCallList(displayList);
		else {
		if(!dont_use_display_lists) {
			displayList = glGenLists(1);
			glNewList(displayList, GL_COMPILE_AND_EXECUTE);
		}
		#endif
		
		#ifdef USE_ARRAYS
		
		#ifndef USE_ES
			if(!dont_use_arrays) drawObjArrays(&model);
			else drawObj(&model);
		#else
		drawObjArrays(&model);
		#endif
		
		#else
		drawObj(&model);
		#endif
		
		#ifndef NO_DISPLAY_LISTS
		if(!dont_use_display_lists)	glEndList();
		}
		#endif
		
	glPopMatrix();
}

void Object::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Object::setOrientation(float newPitch, float newRoll, float newYaw)
{
	orientation.pitch = newPitch;
	orientation.roll = newRoll;
	orientation.yaw = newYaw;
}

void Object::setVelocity(float x, float y, float z)
{
	velocity.x = x;
	velocity.y = y;
	velocity.z = z;
}

void Object::setScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Object::update(float delta_time)
{
	position.x += velocity.x*delta_time;
	position.y += velocity.y*delta_time;
	position.z += velocity.z*delta_time;
}

void Object::cleanupGL()
{
	#ifndef NO_DISPLAY_LISTS
	glDeleteLists(1, displayList);
	#endif
	
	//model.freeModel();
}
