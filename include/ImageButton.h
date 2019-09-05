/*
 ImageButton.h
 ImageButton class definition.

Copyright (c) 2011-2012, 2019 Carlo Retta

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
#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H
#include "Button.h"
#include <Surface.h>

class ImageButton : public Button
{
	unsigned int imageTexture;
	Surface surface;

	void UpdateSurfaceGeometry();
public:
	ImageButton();
	ImageButton(const char *fileName);
	ImageButton(unsigned int texture);
	~ImageButton();
	
	void loadImage(const char *fileName);
	void setTexture(unsigned int texture);
	unsigned int getTexture();
	
	void Draw();
	
	virtual void setPosition(int x, int y){Button::setPosition(x,y);UpdateSurfaceGeometry();}
	virtual void setSize(int width, int height){Button::setSize(width, height);UpdateSurfaceGeometry();}
};
#endif
