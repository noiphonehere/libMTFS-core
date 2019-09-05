/*
 wfmtl.h

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

#ifndef MTL_H
#define MTL_H

#define MAX_MTLS 5	//5 Materials Max.
#define MAX_MTL_PARTS 25
#include <texture.h>

struct OBJMaterial {
	unsigned int diffuse_texture;
	bool has_texture;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	char name[25];
	char diffuse_texture_fname[100];
	imgData *id;
	bool finished_loading;	//False if the texture has been preloaded but not postloaded.
};
class OBJMtlLib {
	int nMtls;
	OBJMaterial materials[MAX_MTLS];
	public:
	char *relativePath;
	OBJMtlLib();
	~OBJMtlLib();
	bool load(const char *fileName);
	OBJMaterial *getMtlByName(const char *n);
};

void mtlFinishLoading(OBJMaterial *mtl);

#endif
