/*
 TextRenderer.cpp
 TextRenderer class implementation file

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

#include "TextRender.h"
#ifndef USE_ES
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#else
#include <GLES/gl.h>
#endif

#ifdef WIN32	//Windows needs glext.h because it only supports core OpenGL 1.1 or something like that
#include <GL/glext.h>
#endif
#include <string.h>
#include <stdlib.h>

TextRenderer::TextRenderer()
{
	drawWidth = 256;
	drawHeight = 32;
	font_point = 16;
	red = green = blue = 1.0f;
	textString = 0;
	cr = 0;
	dX = dY = 0.0;
	isVisible = false;
	
	assignTCoords();
}

TextRenderer::TextRenderer(const char *string)
{
	drawWidth = 256;
	drawHeight = 32;
	cr = 0;
	setText(string);
	font_point = 16;
	red = green = blue = 1.0f;
	dX = dY = 0.0f;
	isVisible = true;
	
	assignTCoords();
}

TextRenderer::~TextRenderer()
{
	glDeleteTextures(1, &glTex);
}

void TextRenderer::assignTCoords()
{
	tCoords[0] = 0; tCoords[1] = 1.0f;
	tCoords[2] = 0; tCoords[3] = 0;
	tCoords[4] = 1.0f; tCoords[5] = 1.0f;
	tCoords[6] = 1.0f; tCoords[7] = 0;
}

void TextRenderer::setText(const char *newText)
{
	resizeBuffer(strlen(newText)+1);
	
	strcpy(textString, newText);
	
	rendered_recently = false;
	isVisible = true;
}

const char *TextRenderer::getText()
{
	return textString;
}

void TextRenderer::setWidth(int newWidth) {drawWidth = newWidth; rendered_recently = false;}

void TextRenderer::setRenderSize(int newWidth, int newHeight)
{
	drawWidth = newWidth;
	drawHeight = newHeight;
	
	rendered_recently = false;
}
void TextRenderer::setColor(float r, float g, float b)
{
	red = r; green = g; blue = b;
	
	rendered_recently = false;
}
void TextRenderer::setFontPoint(int pt) {font_point = pt; rendered_recently = false;}

void TextRenderer::setFont(const char *fontName)
{
	if(!ctxAreInit) initContexts();
	
	pango_font_description_set_family(fdesc, fontName);
	
	rendered_recently = false;
}
/*
void TextRenderer::setSize(int point)
{
}
*/
void TextRenderer::Draw()
{
	if(!isVisible) return;	//Don't render if not visible
	
	if(!rendered_recently) this->Render();
	
	if(!glIsEnabled(GL_TEXTURE_2D)) glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, glTex);
	
	if(dX || dY)
	{
		glPushMatrix();
		glTranslatef(dX, dY, 0);
	}
	
	#ifndef USE_ARRAYS
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(drawWidth, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(drawWidth, drawHeight);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, drawHeight);
	glEnd();
	
	#else
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	glTexCoordPointer(2, GL_FLOAT, 0, tCoords);
	glVertexPointer(2, GL_SHORT, 0, vCoords);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	#endif
	
	if(dX || dY)
	{
		glPopMatrix();
	}
}

bool TextRenderer::initContexts()
{
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, drawWidth, drawHeight);
	cr = cairo_create(surface);
	pctx = pango_cairo_create_context(cr);
	
	fdesc = pango_font_description_from_string("Liberation Sans 14");
	layout = pango_cairo_create_layout(cr);
	
	glGenTextures(1, &glTex);
	glBindTexture(GL_TEXTURE_2D, glTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return true;	
}

void TextRenderer::Render()
{
	if(!cr) initContexts();	//Make sure the contexts are initialized
	
	rendered_recently = true;	//Update the rendered status
	
	//Clear the surface before rendering text to it
	cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.0);
	cairo_paint(cr);
	
	//Render the text in
	cairo_set_source_rgba(cr, red, green, blue, 1.0);
	pango_layout_set_text(layout, textString, strlen(textString));
	pango_font_description_set_size(fdesc, font_point * PANGO_SCALE);
	pango_layout_set_font_description(layout, fdesc);
	pango_layout_set_width(layout, drawWidth * PANGO_SCALE);
	pango_cairo_show_layout(cr, layout);
	
	
	unsigned char *idata = cairo_image_surface_get_data(surface);	//Get the data pointer
	
	//Read it into an OpenGL texture
	glBindTexture(GL_TEXTURE_2D, glTex);
#ifdef USE_ES
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, drawWidth, drawHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, idata);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, drawWidth, drawHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, idata);
#endif
	
	cairo_destroy(cr);	//Uninitialize the context
	cr = 0;	//Set the pointer to zero
	cairo_surface_destroy(surface);	//Destroy the surface
	surface = 0;
	
	vCoords[0] = 0; vCoords[1] = drawHeight;
	vCoords[2] = 0; vCoords[3] = 0;
	vCoords[4] = drawWidth; vCoords[5] = drawHeight;
	vCoords[6] = drawWidth; vCoords[7] = 0;
}

bool TextRenderer::resizeBuffer(int new_size)
{
	if(textString)
		textString = (char*)realloc(textString, new_size);
	else textString = (char*)malloc(new_size);
	
	if(!textString) return false;	//Return false if the buffer failed to resize
	return true;
}

void TextRenderer::setDisplacement(float dX, float dY)
{
	this->dX = dX; this->dY = dY;
}

void TextRenderer::setVisible(bool visible)
{
	isVisible = visible;
}
