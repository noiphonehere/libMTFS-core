/*
 TextRender.h

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

#ifndef TEXT_H
#define TEXT_H

#include <cairo.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

class TextRenderer
{
	protected:
	char *textString;
	cairo_t *cr;
	cairo_surface_t *surface;
	
	PangoContext *pctx;
	PangoFontDescription *fdesc;
	PangoLayout *layout;
	
	unsigned int glTex;
	
	bool ctxAreInit, rendered_recently;
	
	int drawWidth, drawHeight;
	int font_point;
	float dX, dY;
	
	float red, green, blue;
	
	bool isVisible;
	
	float tCoords[8];	//Used with arrays
	short vCoords[8];
	
	bool resizeBuffer(int new_size);

	virtual void Render();
	
	void assignTCoords();
	
	public:
	bool initContexts();
	TextRenderer();
	TextRenderer(const char *string);
	~TextRenderer();
	
	void setText(const char *newText);
	void setWidth(int newWidth);
	virtual void setRenderSize(int newWidth, int newHeight);
	void setColor(float r, float g, float b);
	void setFontPoint(int pt);
	
	const char *getText();
	
	void setFont(const char *fontName);
	void setDisplacement(float dX, float dY);
	void setVisible(bool visible);

	virtual void Draw();
};

#endif
