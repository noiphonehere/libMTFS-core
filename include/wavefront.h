/*
 Wavefront.h

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


#ifndef _WAVEFRONT_H__
#define _WAVEFRONT_H__

#include "wfmtl.h"
#include "wftypes.h"
#include <stdio.h>

#define EX_VERTICES 5
#define EX_TEXCOORDS 5
#define EX_FACES 5
#define EX_PARTS 1

struct OBJArrayData
{
	int count;
	char material[25];
	Point3 *vertices;
	Point3 *normals;
	TexCoord2 *texcoords;
};

class OBJPart
{
	void readVertex(FILE *file);
	void readTexcoord(FILE *file);
	void readNormal(FILE *file);
	void readFace(FILE *file);
	public:
	void *parent;
	void initArrays();
	char *relativePath;
	OBJPart();
	~OBJPart();
	void read(FILE *file, OBJMtlLib *mtllib);
	void free();
	Point3 *vertices;
	Point3 *normals;
	TexCoord2 *texcoords;
	Face3 *faces;
	OBJArrayData array[MAX_MTL_PARTS];
	int nVertices, nNormals, nTexcoords, nFaces, nMtlParts;
	char currentMtlName[25];
};
class OBJFile
{
	char *relativePath;
	public:
	OBJFile();
	~OBJFile();
	int nObjects;
	OBJPart *parts;
	OBJMtlLib mtllib;
	
	bool loadModel(const char *fileName);
	void freeModel();
};

#endif
