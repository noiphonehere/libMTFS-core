/*
 Surface.h
 Declaration of Surface class
 Implementation of simple image surface for quick drawing without having to always create a new array

Copyright (c) 2012 Carlo Retta

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
 

#ifndef SURFACE_H__
#define SURFACE_H__
#include <Button.h>
#include <texture.h>

class Surface
{
	unsigned int texture;
	imgData *id;
	Vector2D pos, size;
	float coords[4][2], texcoords[4][2];
	unsigned char indices[6];
	void updateCoords();
public:
	Surface();
	Surface(float xPos, float yPos, float w, float h);
	~Surface();
	void loadImage(const char *imageName);
	void freeImage();
	void setImage(unsigned int image){texture = image;}
	unsigned int getImage(){return texture;}
	void setPosition(float xPos, float yPos) {pos.x = xPos; pos.y=yPos;}
	void setSize(float w, float h) {size.x = w; size.y=h;}
	void setImageData(imgData *newID);
	Vector2D *getPosition() {return &pos;}
	Vector2D *getSize() {return &size;}
	virtual void Draw();
};

#endif
