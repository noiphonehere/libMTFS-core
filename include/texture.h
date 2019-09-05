/*
 Texture.h

Copyright (c) 2010-2012 Carlo Retta

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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>

static const char * TEXLIB_VERSION = "texture.c v.0.5.0 Copyright 2010-2012 Carlo Retta";
struct imgData {
	int origW, origH;
	int newW, newH;
	
	SDL_Surface *pSurface;	//This pointer is valid after preloading but not after postloading.
	unsigned int tex;	//OpenGL Texture number. Zero if not postloaded
};
unsigned int loadTexture(const char *fileName, struct imgData *id = 0);

imgData *preloadTexture(const char *fileName);	//Need this so that loading can happen on a different thread. No OpenGL calls allowed in this function!
unsigned int postloadTexture(imgData *preloadedTexture);	//OpenGL part of loading goes here. Must be done on main thread.
#endif
