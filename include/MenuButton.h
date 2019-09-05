/*
 MenuButton.h

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
#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include "ImageButton.h"
#include "TextButton.h"

enum TextLabelPosition {
	TOP,
	LEFT,
	RIGHT,
	BOTTOM
};

class MenuButton : public ImageButton
{
	int nItems;
	TextButton **items;	//An array of text buttons for the menu items
	
	bool menuIsVisible;
	float itemYPosMult;		//Extent to which the menu is open (for animations)
	
	char *labelText;
	TextRenderer lr;		//TextRenderer to draw the label
	TextLabelPosition textPos;
	
	TextButton **makeNewArray();	//Function to increase the array size by one
	
public:
	MenuButton();
	~MenuButton();
	
	void addMenuItem(TextButton *tb);
	void quickAddMenuItem(const char *title, void (*onclick)(Button*));	//Add by title and callback
	void deleteMenuItem(int index);	//TODO: Not implemented
	
	void Draw(int include_menu = 1);
	
	int getNumItems(){return nItems;}
	TextButton **getItemList(){return items;}
	
	virtual bool processClick(int x, int y, int button = LEFT_CLICK);
	
	virtual void setPosition(int x, int y);
	virtual void setLabel(char *newText){lr.setText(newText);}
	virtual const char *getLabel(){return lr.getText();}
	
	void setTextLabelPosition(TextLabelPosition pos);
	
	void update(float delta_time);	//Used to update the animation
};
#endif
