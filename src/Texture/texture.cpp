/*
 texture.cpp
 Texture loader using SDL_image

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

#include "texture.h"

#ifndef USE_ES
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#else
#include <GLES/gl.h>
#endif

#ifdef USE_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif

#include <math.h>
#include <memory.h>
inline unsigned int MAX(unsigned int a, unsigned int b){
if(a>b)return a;
else return b;
}

unsigned int loadTexture(const char *fileName, struct imgData *id)
{
	int i, j;
	
	#ifdef DEBUG_INFO
	printf("Loading %s....", fileName);
	#endif
	
	unsigned int tex;
	SDL_Surface *image = NULL;
	
	image = IMG_Load(fileName);
	if(image == NULL)
	{
		printf("fatal:image is NULL. (%s)\n", fileName);
		return 0;
	}
	
	#ifdef DEBUG_INFO
	else printf("loaded...");
	#endif
	
	#ifdef DEBUG_INFO
	printf("new texture...");
	#endif
	glGenTextures(1, &tex);
	
	if(id){
	id->origW = id->newW = image->w;
	id->origH = id->newH = image->h;
	}
	
	for(i=2;i<image->w;i*=2);	//calculate the next highest power-of-two size (if not power of two already)
	for(j=2;j<image->h;j*=2);
	
	//if(i != image->w || j != image->h)
	//{
		if(id){
			id->newW = i;
			id->newH = j;
		}
	//}
	
	//Make sure the format is correct to load into the OpenGL texture
	#ifdef DEBUG_INFO
	printf("new size %dx%d...", i, j);
	#endif
	Uint32 RMask, GMask, BMask, AMask;
	int bpp;
	#ifdef USE_SDL2
	SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp, &RMask, &GMask, &BMask, &AMask);
	#else
	bpp = MAX(24,image->format->BitsPerPixel);
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		AMask = 0x000000ff;
		RMask = 0xff000000;
		GMask = 0x00ff0000;
		BMask = 0x0000ff00;
	}
	else {
		AMask = 0xff000000;
		RMask = 0x000000ff;
		GMask = 0x0000ff00;
		BMask = 0x00ff0000;
	}
	#endif
	SDL_Surface *temp;
	int alpha_enabled;
	if(image->format->BitsPerPixel == 32) alpha_enabled=0xffffffff;
	else alpha_enabled = 0;
	//if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	#ifdef PPC_DRI_ENDIAN_WORKAROUND
		temp = SDL_CreateRGBSurface(0, i, j, bpp, AMask, BMask, GMask, RMask);
	#else
		temp = SDL_CreateRGBSurface(0, i, j, bpp, RMask, GMask, BMask, AMask);
	#endif
		/*temp = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ABGR8888, 0);*/	//new surface
	//else
	//	temp = SDL_CreateRGBSurface(0, i, j, MAX(24,image->format->BitsPerPixel), 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000&alpha_enabled);/*temp = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);*/

#ifdef USE_SDL2
	SDL_SetSurfaceAlphaMod(image, 0xFF);
	SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_NONE);
#else
	SDL_SetAlpha(image, 0, 0);
#endif

	SDL_BlitSurface(image, NULL, temp, NULL);
		
	SDL_FreeSurface(image);	//free old image surface
	image = temp;	//replace pointer so rest of function uses new image
	
	//Create the texture and set up its options
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	#ifdef DEBUG_INFO
	printf("set texture...");
	#endif
	//Use the right format
	int imageFormat;
	switch( MAX(24,image->format->BitsPerPixel) )
	{
	case 24: imageFormat = GL_RGB; 
	#ifdef DEBUG_INFO
	printf("(RGB)...");
	#endif
	break;
	case 32: imageFormat = GL_RGBA;
	#ifdef DEBUG_INFO
	printf("(RGBA)...");
	#endif
	break;
	default: imageFormat = GL_RGB;	//To be safe, so that it will hopefully not overrun.
	break;
	}
	SDL_LockSurface(image);
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, image->w, image->h, 0, imageFormat, GL_UNSIGNED_BYTE, image->pixels);
	SDL_UnlockSurface(image);
	#ifdef DEBUG_INFO
	printf("done.\n");
	#endif
	//Free the image surface
	SDL_FreeSurface(image);
	return tex;
}

imgData *preloadTexture(const char *fileName)
{
		int i, j;
	
	#ifdef DEBUG_INFO
	printf("Loading %s....", fileName);
	#endif
	
	imgData *id = new imgData;
	if(!id) return NULL;
	memset(id, 0, sizeof(imgData));
	id->tex = 0;	//We will not load the texture in this function
	
	SDL_Surface *image = NULL;
	
	image = IMG_Load(fileName);
	if(image == NULL)
	{
		printf("fatal:image is NULL. (%s)\n", fileName);
		return 0;
	}
	
	#ifdef DEBUG_INFO
	else printf("loaded...");
	#endif
	
	#ifdef DEBUG_INFO
	printf("new texture...");
	#endif
	
	
	
	id->origW = id->newW = image->w;	//Initialize the size fields
	id->origH = id->newH = image->h;
	
	
	for(i=2;i<image->w;i*=2);	//calculate the next highest power-of-two size (if not power of two already)
	for(j=2;j<image->h;j*=2);
	
	
	id->newW = i;			//Set the adjusted size
	id->newH = j;
	
	
	//Make sure the format is correct to load into the OpenGL texture
	#ifdef DEBUG_INFO
	printf("new size %dx%d...", i, j);
	#endif
	Uint32 RMask, GMask, BMask, AMask;
	int bpp;
	#ifdef USE_SDL2
	SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp, &RMask, &GMask, &BMask, &AMask);
	#else
	bpp = MAX(24,image->format->BitsPerPixel);
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		AMask = 0x000000ff;
		RMask = 0xff000000;
		GMask = 0x00ff0000;
		BMask = 0x0000ff00;
	}
	else {
		AMask = 0xff000000;
		RMask = 0x000000ff;
		GMask = 0x0000ff00;
		BMask = 0x00ff0000;
	}
	#endif
	SDL_Surface *temp;
	int alpha_enabled;
	if(image->format->BitsPerPixel == 32) alpha_enabled=0xffffffff;
	else alpha_enabled = 0;
	//if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	#ifdef PPC_DRI_ENDIAN_WORKAROUND
		temp = SDL_CreateRGBSurface(0, i, j, bpp, AMask, BMask, GMask, RMask);
	#else
		temp = SDL_CreateRGBSurface(0, i, j, bpp, RMask, GMask, BMask, AMask);
	#endif
		/*temp = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ABGR8888, 0);*/	//new surface
	//else
	//	temp = SDL_CreateRGBSurface(0, i, j, MAX(24,image->format->BitsPerPixel), 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000&alpha_enabled);/*temp = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);*/

#ifdef USE_SDL2
	SDL_SetSurfaceAlphaMod(image, 0xFF);
	SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_NONE);
#else
	SDL_SetAlpha(image, 0, 0);
#endif
	SDL_BlitSurface(image, NULL, temp, NULL);
		
	SDL_FreeSurface(image);	//free old image surface
	image = temp;	//replace pointer so rest of function uses new image
	
	id->pSurface = image; //Set the surface pointer in the structure
	
	return id;	//Return the data structure
}

unsigned int postloadTexture(imgData *preloadedTexture)
{
	if(!preloadedTexture || !preloadedTexture->pSurface) return 0;
	glGenTextures( 1, &(preloadedTexture->tex) );
	
	//Create the texture and set up its options
	glBindTexture(GL_TEXTURE_2D, preloadedTexture->tex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	#ifdef DEBUG_INFO
	printf("set texture...");
	#endif
	//Use the right format
	int imageFormat;
	switch( MAX(24,preloadedTexture->pSurface->format->BitsPerPixel) )
	{
	case 24: imageFormat = GL_RGB; 
	#ifdef DEBUG_INFO
	printf("(RGB)...");
	#endif
	break;
	case 32: imageFormat = GL_RGBA;
	#ifdef DEBUG_INFO
	printf("(RGBA)...");
	#endif
	break;
	default: imageFormat = GL_RGB;	//To be safe, so that it will hopefully not overrun.
	break;
	}
	SDL_LockSurface(preloadedTexture->pSurface);
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, preloadedTexture->pSurface->w, preloadedTexture->pSurface->h, 0, imageFormat, GL_UNSIGNED_BYTE, preloadedTexture->pSurface->pixels);
	SDL_UnlockSurface(preloadedTexture->pSurface);
	#ifdef DEBUG_INFO
	printf("done.\n");
	#endif
	//Free the image surface
	SDL_FreeSurface(preloadedTexture->pSurface);	//TODO: Possibly allow an option to avoid freeing this and leave it up to the caller
	preloadedTexture->pSurface = 0;
	return preloadedTexture->tex;
}
