/*
 DescriptionRenderer.h
 Created by Carlo Retta on 7/4/12.

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

#include <TextRender.h>
#include <Surface.h>

class DescriptionRenderer : public TextRenderer
{
	void regenVCoords();
	
protected:
	unsigned int borderTexImage, triedToLoadBTI;
	unsigned int profileImage;
	Surface profileSurface;
	virtual void Render();
	
public:
	DescriptionRenderer();
	DescriptionRenderer(const char *string);
	
	virtual void Draw();
	virtual void setProfileImage(unsigned int tex);
	virtual void setProfileImage(imgData *id);
	virtual void setRenderSize(int newWidth, int newHeight) { TextRenderer::setRenderSize(newWidth, newHeight); regenVCoords(); }
};
