/*
 Button.h
 Button class declaration
 
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
#ifndef BUTTON_H
#define BUTTON_H

#define LEFT_CLICK 0
#define RIGHT_CLICK 1

#include <MTFS-Types.h>

class Button
{
protected:
	Vector2D position;
	Vector2D size;
	
	void (*onclick)(Button *btn);
	void (*onrightclick)(Button *btn);
	
	int btnID;
	bool hover;
	int vCoords[8];
	float tCoords[8];
	
	void regenCoords();
	void assignTCoords();
	
public:
	void *assocPtr;
	
	Button(int x, int y, int width, int height);
	Button(Vector2D *pos, Vector2D *size);
	Button();
	~Button();
	
	virtual void setPosition(int x, int y);
	virtual void setSize(int width, int height);
	Vector2D *getPosition();
	Vector2D *getSize();
	
	void setOnClick(void (*onclick)(Button*));
	void setOnRightClick(void (*onrightclick)(Button*));
	
	void setID(int newID);
	int getID();
	
	virtual bool processClick(int x, int y, int button = LEFT_CLICK);
	virtual void processMouseMove(int x, int y);
	
	virtual void Draw();
};

#endif
