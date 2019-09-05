/*
 ImageButton.cpp
 ImageButton class implementation file

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

#include "ImageButton.h"
#include <texture.h>
#ifndef USE_ES
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#else
#include <GLES/gl.h>
#endif

ImageButton::ImageButton(): imageTexture(0) {}
ImageButton::ImageButton(const char *fileName) : imageTexture(0)
{
	glDeleteTextures(1, &imageTexture);	//Delete any old texture first
	imageTexture = loadTexture(fileName);
}
ImageButton::ImageButton(unsigned int texture)
{
	glDeleteTextures(1, &imageTexture);	//Delete any old texture first
	imageTexture = texture;
}
ImageButton::~ImageButton()
{
	glDeleteTextures(1, &imageTexture);	//Delete the texture object we were using
}

void ImageButton::loadImage(const char *fileName)
{
	#ifdef REVERT_SURFACE_CONVERSION
	glDeleteTextures(1, &imageTexture);	//Delete any old texture first
	imageTexture = loadTexture(fileName);
	#else
	surface.loadImage(fileName);
	#endif
}

void ImageButton::setTexture(unsigned int texture)
{
	#ifdef REVERT_SURFACE_CONVERSION
	glDeleteTextures(1, &imageTexture);	//Delete any old texture first
	imageTexture = texture;
	#else
	surface.setImage(texture);
	#endif
}

unsigned int ImageButton::getTexture()
{
	#ifdef REVERT_SURFACE_CONVERSION
	return imageTexture;
	#else
	return surface.getImage();
	#endif
}

void ImageButton::Draw()
{
	#ifdef REVERT_SURFACE_CONVERSION
		if(!glIsEnabled(GL_TEXTURE_2D)) glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, imageTexture);
	
		#ifndef USE_ARRAYS
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(position.x, position.y);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(position.x+size.x, position.y);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(position.x+size.x, position.y+size.y);
				glTexCoord2f(0.0f, 1.0f); glVertex2i(position.x, position.y+size.y);
			glEnd();
		#else	//We are assuming that vertex arrays have already been enabled
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, tCoords);
			glVertexPointer(2, GL_INT, 0, vCoords);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		#endif
	
	#else
		surface.Draw();
	#endif
}

#ifndef REVERT_SURFACE_CONVERSION
void ImageButton::UpdateSurfaceGeometry()
{
	surface.setPosition(position.x, position.y);
	surface.setSize(size.x, size.y);
}
#endif
