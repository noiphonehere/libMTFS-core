/*
 MenuButton.cpp
 MenuButton class implementation file

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
#include "MenuButton.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>

MenuButton::MenuButton() : nItems(0), items(0), menuIsVisible(false), labelText(0), itemYPosMult(0.0f), textPos(TOP)
{
}

MenuButton::~MenuButton()
{
	if(items)
		free(items);
}

TextButton **MenuButton::makeNewArray()
{
	TextButton **tempArray;
	
	if(items)
		tempArray = (TextButton**)realloc(items, sizeof(TextButton*)*(nItems+1));
		
	else tempArray = (TextButton**)malloc(sizeof(TextButton*)*(nItems+1));
	
	return tempArray;
}

void MenuButton::addMenuItem(TextButton *tb)
{
	//Note that since arrays are 0-based, items[nItems] is the new item *before* it is incremented!
	items = makeNewArray();
	
	memcpy((void*)(items[nItems]), (void*)tb, sizeof(TextButton));	//This might not actually work...
	
	items[nItems]->setPosition(position.x, position.y-(nItems+1)*32);
	
	nItems++;
}

void MenuButton::quickAddMenuItem(const char *title, void (*onclick)(Button*))
{
	items = makeNewArray();
	
	items[nItems] = new TextButton();
	
	items[nItems]->setText(title);
	items[nItems]->setOnClick(onclick);
	items[nItems]->setSize(64, 32);
	items[nItems]->setPosition(position.x, position.y-(nItems+1)*32);
	
	nItems++;
}

void MenuButton::deleteMenuItem(int index)
{
	//Not implemented
}

bool MenuButton::processClick(int x, int y, int button)
{
	int i;
	
	if(!items) return Button::processClick(x,y,button);	//Act like a normal button unless have a list of items
	
	if(Button::processClick(x,y,button))
	{
		menuIsVisible = !menuIsVisible;	//Toggle visibility of menu
		return true;
	}
	else
	{
		if(menuIsVisible)
		{
			menuIsVisible = false;
			
			for(i=0;i<nItems;i++)
			{
				if(!items[i]) continue;
				else if(items[i]->processClick(x,y,button)) return true;
			}
		}
	}
	
	return false;
}


void MenuButton::Draw(int include_menu)
{
	int i;
	
	//Draw the menu first so that when it is closing it can go below the button
	if( (menuIsVisible || itemYPosMult) && include_menu)
	{
		for(i=0;i<nItems;i++)
		{
			items[i]->Draw();
		}
	}
	else if(!menuIsVisible && include_menu)
	{
		if(hover)
		{
			lr.Draw();
		}
	}
	
	ImageButton::Draw();
}
void MenuButton::setPosition(int x, int y)
{
	int i;
	
	ImageButton::setPosition(x,y);
	regenCoords();
	
	setTextLabelPosition(textPos);	//Reset the text label position
	
	if(!items) return;
	
	for(i=0;i<nItems;i++)	//Adjust the positions of the menu items (is this necessary anymore?)
	{
		if(items[i]) items[i]->setPosition(x, y-(i+1)*32.0f);
	}
}
void MenuButton::setTextLabelPosition(TextLabelPosition pos)
{
	textPos = pos;
	
	switch(pos)
	{
		case TOP:
			lr.setDisplacement(position.x, position.y-24.0f);
			break;
			
		case RIGHT:
			lr.setDisplacement(position.x+size.x, position.y);
			break;
			
		case LEFT:
			lr.setDisplacement(position.x-64.0f, position.y);
			break;
			
		case BOTTOM:
			lr.setDisplacement(position.x, position.y+size.y);
			break;
			
		default:
			break;
	}
}
void MenuButton::update(float delta_time)
{
	int i;
	
	if(menuIsVisible && itemYPosMult != 1.0f)	//Slide up the menu items
	{
		itemYPosMult += ( 4.0f*delta_time * cosf( 
						itemYPosMult*(M_PI/2.1f)
							) );
							
		if(itemYPosMult > 1.0f) itemYPosMult = 1.0f;
		
		for(i=0;i<nItems;i++)
		{
			if(items[i]) items[i]->setPosition(position.x, position.y - (i+1)* 32.0f * itemYPosMult);
		}
		
	}
	else if(!menuIsVisible && itemYPosMult)		//Slide down the menu items
	{
		itemYPosMult -= 4.0f*delta_time * sinf( itemYPosMult*(M_PI/2.1f) ) + delta_time/4.0f;
		
		if(itemYPosMult < 0.0f) itemYPosMult = 0.0f;
		
		for(i=0;i<nItems;i++)
		{
			if(items[i]) items[i]->setPosition(position.x, position.y - (i+1)*32.0f * itemYPosMult);
		}
		
	}
}
