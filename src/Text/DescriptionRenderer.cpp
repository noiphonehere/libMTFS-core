/*
 DescriptionRenderer.cpp 
 DescriptionRenderer class implementation file
 Created by Carlo Retta on 7/4/12.

 Copyright (c) 2012, 2019 Carlo Retta.

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

#include "DescriptionRenderer.h"
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


float bkgTexCoords[16][2] = {
	{0.0f, 0.0f}, {0.1f, 0.0f},	{0.9f, 0.0f}, {1.0f, 0.0f},
	{0.0f, 0.1f}, {0.1f, 0.1f},	{0.9f, 0.1f}, {1.0f, 0.1f},
	{0.0f, 0.9f}, {0.1f, 0.9f},	{0.9f, 0.9f}, {1.0f, 0.9f},
	{0.0f, 1.0f}, {0.1f, 1.0f},	{0.9f, 1.0f}, {1.0f, 1.0f}
};

float bkgVCoords[16][2] = {		//This side depends on size of DescriptionRenderer, but we'll assume 128x128 initially
	{0.0f, 0.0f},   {16.0f, 0.0f},	{112.0f, 0.0f},  {128.0f, 0.0f},
	{0.0f, 16.0f},  {16.0f, 16.0f},	{112.0f, 16.0f}, {128.0f, 16.0f},
	
	{0.0f, 112.0f}, {16.0f, 112.0f},{112.0f, 112.0f},{128.0f, 112.0f},
	{0.0f, 128.0f}, {16.0f, 128.0f},{112.0f, 128.0f},{128.0f, 128.0f}
};

unsigned char bkgIndices[54] =
{
	4, 0, 1,  4, 1, 5,
	5, 1, 2,  5, 2, 6,
	6, 2, 3,  6, 3, 7,
	
	8, 4, 5,  8, 5, 9,
	9, 5, 6,  9, 6, 10,
	10, 6, 7,  10, 7, 11,
	
	12, 8, 9,  12, 9, 13,
	13, 9, 10,  13, 10, 14,
	14, 10, 11,  14, 11, 15
};

DescriptionRenderer::DescriptionRenderer() : borderTexImage(0), profileImage(0)
{
	profileSurface.setSize(64.0f, 64.0f);
}

DescriptionRenderer::DescriptionRenderer(const char *string) : borderTexImage(0), profileImage(0)
{
	profileSurface.setSize(64.0f, 64.0f);
	setText(string);
}

void DescriptionRenderer::regenVCoords()
{
	float w9 = drawWidth;
	float w  = drawWidth+16.0f;
	float h9 = drawHeight;
	float h = drawHeight+16.0f;
	
	//Do not do what I do. This code is awful!
	bkgVCoords[2][0] = w9; bkgVCoords[3][0] = w;
	bkgVCoords[6][0] = w9; bkgVCoords[7][0] = w;
	bkgVCoords[8][1] = bkgVCoords[9][1] = bkgVCoords[10][1] = bkgVCoords[11][1] = h9;
	bkgVCoords[10][0] = w9; bkgVCoords[11][0] = w;
	bkgVCoords[12][1] = bkgVCoords[13][1] = bkgVCoords[14][1] = bkgVCoords[15][1] = h;
	bkgVCoords[14][0] = w9; bkgVCoords[15][0] = w;
}
void DescriptionRenderer::Render()
{
	if(!borderTexImage) borderTexImage = loadTexture("Data/Graphics/DescriptionBkg.png");
	
	TextRenderer::Render();
	
	regenVCoords();
}
void DescriptionRenderer::setProfileImage(unsigned int tex)
{
	profileImage = tex;
	profileSurface.setImage(tex);
	profileSurface.setPosition(dX, dY);
}
void DescriptionRenderer::setProfileImage(imgData *id)
{
	profileImage = id->tex;
	profileSurface.setImageData(id);
	profileSurface.setPosition(dX, dY);
}
void DescriptionRenderer::Draw()
{
	regenVCoords();
	if(!borderTexImage && !triedToLoadBTI) {
		borderTexImage = loadTexture("Data/Graphics/DescriptionBkg.png");
		triedToLoadBTI = true;
	}
	if(!rendered_recently) Render();
	
	glPushMatrix();
	
		glTranslatef(dX, dY-32.0f, 0.0f);
	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, borderTexImage);
	
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, bkgVCoords);
		glTexCoordPointer(2, GL_FLOAT, 0, bkgTexCoords);
	
		glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_BYTE, bkgIndices);
	
	glPopMatrix();
	
	if(profileImage) {
		profileSurface.Draw();
	}
	
	glPushMatrix();
		if(profileImage) glTranslatef(0.0f, 64.0f, 0.0f);
		
		TextRenderer::Draw();
	glPopMatrix();
}
