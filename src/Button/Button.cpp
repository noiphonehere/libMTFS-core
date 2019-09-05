/*
 Button.cpp
 Button class implementation file

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

#include "Button.h"
#include <memory.h>

Button::Button() : assocPtr(0), hover(false)
{
	assignTCoords();
}

Button::Button(Vector2D *pos, Vector2D *size) : assocPtr(0), hover(false)
{
	memcpy(&position, pos, sizeof(Vector2D));
	memcpy(&(this->size), size, sizeof(Vector2D));
	
	regenCoords();
	assignTCoords();
}

Button::Button(int x, int y, int width, int height) : assocPtr(0), hover(false)
{
	position.x = x; position.y = y;
	size.x = width; size.y = height;
	
	regenCoords();
	assignTCoords();
}

Button::~Button() {}

void Button::setPosition(int x, int y)
{
	position.x = x; position.y = y;
	regenCoords();
}

void Button::setSize(int width, int height)
{
	size.x = width; size.y = height;
	regenCoords();
}

void Button::setOnClick(void (*onclick)(Button*))
{
	this->onclick = onclick;
}

void Button::setOnRightClick(void (*onrightclick)(Button*))
{
	this->onrightclick = onrightclick;
}

Vector2D *Button::getPosition()
{
	return &position;
}

Vector2D *Button::getSize()
{
	return &size;
}

void Button::setID(int newID)
{
	btnID = newID;
}

int Button::getID()
{
	return btnID;
}

bool Button::processClick(int x, int y, int button)
{
	if(x < position.x || x > position.x+size.x) return false;
	if(y < position.y || y > position.y+size.y) return false;
	
	if(button == RIGHT_CLICK && onrightclick) onrightclick(this);
	else if(onclick) onclick(this);
	return true;
}

void Button::processMouseMove(int x, int y)
{
	if(x < position.x || x > position.x+size.x) {hover = false; return;}
	if(y < position.y || y > position.y+size.y) {hover = false; return;}
	
	hover = true;
	return;
}

void Button::Draw()
{
}

void Button::regenCoords()
{
	vCoords[0] = position.x; vCoords[1] = position.y+size.y;
	vCoords[2] = position.x; vCoords[3] = position.y;
	vCoords[4] = position.x+size.x; vCoords[5] = position.y+size.y;
	vCoords[6] = position.x+size.x; vCoords[7] = position.y;
}

void Button::assignTCoords()
{
	tCoords[0] = 0; tCoords[1] = 1.0f;
	tCoords[2] = 0; tCoords[3] = 0;
	tCoords[4] = 1.0f; tCoords[5] = 1.0f;
	tCoords[6] = 1.0f; tCoords[7] = 0;
}
