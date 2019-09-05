/*
 Surface.cpp
 Surface class implementation file

 Copyright (c) 2012, 2019 Carlo Retta

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

#include "Surface.h"
#include <memory.h>
#include <texture.h>
#include <stdio.h>

#ifndef USE_ES
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#else
#include <GLES/gl.h>
#endif
	//Default coordinate information
float defcoords[4][2] = {
	{0.0f, 1.0f}, {0.0f, 0.0f},
	{1.0f, 0.0f}, {1.0f, 1.0f}
};
unsigned char defindices[6] = {
	0, 1, 2,
	0, 2, 3
};

void Surface::setImageData(imgData *newID)	//As I remember, this does not work right
{
	if(!id) id = new imgData;	//Make sure we do not try to copy to a NULL pointer. Maybe this is why the crash...
	
	memcpy(id, newID, sizeof(imgData));	//Copy in the new image data
	
	texture = id->tex;	//Extract the texture number
	
	updateCoords();	//Update our coordinates
}

void Surface::updateCoords()
{
	//Reset the local copy of the coordinate information with the default information
	memcpy(coords, defcoords, sizeof(float)*8);
	memcpy(texcoords, defcoords, sizeof(float)*8);
	memcpy(indices, defindices, sizeof(unsigned char)*6);
	
	if(id && id->newW != 0 && id->newH != 0) {	//No segfault, no div. by zero
		texcoords[2][0] = texcoords[3][0] = ((float)(id->origW))/((float)(id->newW));
		texcoords[0][1] = texcoords[3][1] = ((float)(id->origH))/((float)(id->newH));
	}
}

Surface::Surface() : texture(0)
{
	id = 0;
	memset(&pos, 0, sizeof(Vector2D));
	memset(&size, 0, sizeof(Vector2D));
	updateCoords();
}

Surface::Surface(float xPos, float yPos, float w, float h) : texture(0)
{
	id = 0;
	pos.x = xPos; pos.y = yPos;
	size.x = w; size.y = h;
	updateCoords();
}

Surface::~Surface()
{
	if(id) delete id;	//Free the imgData structure
}

void Surface::loadImage(const char *imageName)
{
	#ifndef USE_PRELOADER
	if(!id) id = new imgData;	//Allocate info
	texture = loadTexture(imageName, id);
	if(texture == 0) printf("Texture not loaded!\n");
	#else
	id = preloadTexture(imageName);
	#endif
	updateCoords();
}

void Surface::freeImage()
{
	glDeleteTextures(1, &texture);
}

void Surface::Draw()
{
	#ifdef USE_PRELOADER
	if(id && !id->tex && id->pSurface) {
		postloadTexture(id);
		texture = id->tex;
	}
	#endif
	glPushMatrix();
	
	glTranslatef(pos.x, pos.y, 0.0f);	//Move into position and adjust size
	glScalef(size.x, size.y, 1.0f);
	
	if(!glIsEnabled(GL_TEXTURE_2D)) glEnable(GL_TEXTURE_2D);		//Make sure textures are enabled
	
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glVertexPointer(2, GL_FLOAT, 0, coords);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
	
	glPopMatrix();
}
