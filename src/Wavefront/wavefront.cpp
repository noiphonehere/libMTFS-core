/*
 wavefront.cpp
 Routines for loading a Wavefront OBJ file

 Copyright (c) 2010-2012, 2019 Carlo Retta

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

#include "wavefront.h"
#include <stdio.h>
#include <algorithm>

template<class T> void realloc_mem(int old_size, int new_size, T **ptr)
{
	#ifdef DEBUG_INFO
	printf("Creating new buffer of size %d.\n", new_size);
	#endif
	T *temp = new T[new_size];//Allocate temporary buffer that is the desired new size
	#ifdef DEBUG_INFO
	printf("Copying memory\n");
	#endif
	memcpy(temp, *ptr, std::min(new_size,old_size)*sizeof(T));//copy the memory to the buffer
	
	delete[] *ptr;//Delete the old buffer
	*ptr = temp;	//Point the old pointer to the temporary buffer
}

OBJFile::OBJFile()
{
	parts = new OBJPart[EX_PARTS];
}
OBJFile::~OBJFile()
{
	if(parts) {
		freeModel();
		//delete[] parts;
		}
	if(relativePath) delete [] relativePath;
}
void OBJFile::freeModel()
{
	for(int i = 0;i<nObjects;i++)
	{
		parts[i].free();
	}
}

bool OBJFile::loadModel(const char *fileName)
{
	FILE *file = fopen(fileName, "r");
	char *slash_pos = strrchr((char*)fileName, '/');
	if(slash_pos)
	{
		relativePath = new char[slash_pos-fileName+2];
		memset(relativePath, 0, slash_pos-fileName+2);
		strncpy(relativePath, fileName, slash_pos-fileName+1);
		#ifdef DEBUG_INFO
		printf("Relative path is: %s\n", relativePath);
		#endif
	}
	int i = 0;
	while(!feof(file)){
	if(i>=EX_PARTS) return true;// realloc_mem(i,i+1,OBJPart,&parts);
	parts[i].relativePath = relativePath;
	parts[i].parent = (void*)this;
	parts[i].read(file,&mtllib);
	i++;
	nObjects = i;
	}
	parts[0].initArrays();
	return true;
}

void OBJPart::read(FILE *file, OBJMtlLib *mtllib)
{
	char tmp,tmp2;
	char tmps[100];
	char tmps2[100];
	//int mode = 0;	//0=vertices 1=faces
	
	while(1){
		tmp = fgetc(file);
		
		switch(tmp)
		{
		case '#':
			fgets(tmps,99,file);
			break;
		case 'v':
			/*if(mode == 1)	//I believe this code is unreachable
			{
				ungetc(tmp,file);
				printf("%d faces read.\n",nFaces);
				return;
			}*/
			tmp2 = fgetc(file);
			switch(tmp2)
			{
				case ' ': readVertex(file); break;
				case 't': readTexcoord(file); break;
				case 'n': readNormal(file); break;
			}
			break;
		case 'f':
			//mode = 1;
			//tmp2 = fgetc(file);
			//if(tmp != ' ') ungetc(tmp2,file);
			readFace(file);
			break;
		case 'm':
			ungetc(tmp,file);
			fscanf(file,"%99s",tmps);
			if(!strcmp("mtllib",tmps))
			{
				fscanf(file,"%99s",tmps);
				memset(tmps2, 0, 100);
				//if(relativePath) strcpy(tmps2, relativePath);
				//strcat(tmps2, tmps);
				mtllib->relativePath = relativePath;
				mtllib->load(tmps);
			}
			break;
		case 'u':
			ungetc(tmp,file);
			fscanf(file, "%99s",tmps);
			if(!strcmp("usemtl",tmps))
			{
				fgets(currentMtlName,24,file);
			}
			break;
		default:
			//Error or unsupported feature
			if(tmp == 's')	
			{
				#ifdef DEBUG_INFO
				printf("OBJLOADER WARNING: You have to be an idiot not to want smooth shading.\n");
				#endif
				fgets(tmps,99,file);
			}
			else if(tmp == EOF) {printf("OBJLOADER NOTE: End of file reached.\n"); return;}
			else if(tmp == '\n' || tmp == 'o') break;
			else if(tmp == 'g') {/* printf("OBJLOADER NOTE: I don\'t group objects.\n");*/ fgets(tmps,99,file); tmp2 = fgetc(file); if(tmp2 != 13 && tmp != '\n') ungetc(tmp2,file); }
			else {fgets(tmps,99,file);/*printf("OBJLOADER WARNING: Unrecognized line starts with %d at pos %d.\n%s\n",(int)tmp,ftell(file),tmps);*/ }
			break;
		}
		
		tmp = fgetc(file);
		if(feof(file)) return;
		if(tmp != '\n' && tmp != 13) ungetc(tmp,file);
	}
}
OBJPart::OBJPart()
{
nVertices = nNormals = nTexcoords = nFaces = 0;
vertices = new Point3[EX_VERTICES];
normals = new Point3[EX_VERTICES];
texcoords = new TexCoord2[EX_TEXCOORDS];
faces = new Face3[EX_FACES];
memset(vertices, 0, sizeof(Point3)*EX_VERTICES);
memset(normals, 0, sizeof(Point3)*EX_VERTICES);
memset(texcoords, 0, sizeof(TexCoord2)*EX_VERTICES);
memset(faces, 0, sizeof(Face3)*EX_VERTICES);
}
OBJPart::~OBJPart()
{
	if(vertices)
	delete[] vertices;
	if(normals)
	delete[] normals;
	if(texcoords)
	delete[] texcoords;
	if(faces)
	delete[] faces;
	
	vertices = 0;
	normals = 0;
	texcoords = 0;
	faces = 0;
}
void OBJPart::readVertex(FILE *file)
{
	fscanf(file, "%f %f %f\n", &vertices[nVertices].x, &vertices[nVertices].y, &vertices[nVertices].z);
	nVertices++;
	if(nVertices >= EX_VERTICES)
	{
		realloc_mem<Point3>(nVertices, nVertices+1, &vertices);
	}
}

void OBJPart::readNormal(FILE *file)
{
	fscanf(file, "%f %f %f\n", &normals[nNormals].x, &normals[nNormals].y, &normals[nNormals].z);
	nNormals++;
	if(nNormals >= EX_VERTICES)
	{
		realloc_mem<Point3>(nNormals, nNormals+1, &normals);
	}
}

void OBJPart::readTexcoord(FILE *file)
{
	fscanf(file, "%f %f\n", &texcoords[nTexcoords].s, &texcoords[nTexcoords].t);
	texcoords[nTexcoords].t *= -1.0f;
	nTexcoords++;
	if(nTexcoords >= EX_VERTICES)
	{
		realloc_mem<TexCoord2>(nTexcoords, nTexcoords+1, &texcoords);
	}
}

void OBJPart::readFace(FILE *file)
{
	char tmp = 0;//, tmp2 = 0;
	/*fscanf(file, "%d//%d %d//%d %d//%d",
	&faces[nFaces].vIndices[0], &faces[nFaces].tIndices[0], &faces[nFaces].nIndices[0],
	&faces[nFaces].vIndices[1], &faces[nFaces].tIndices[1], &faces[nFaces].nIndices[1],
	&faces[nFaces].vIndices[2], &faces[nFaces].tIndices[2], &faces[nFaces].nIndices[2] );*/
	tmp = fgetc(file);
	//printf("%c:",tmp);
	for(int i = 0;i<3;i++){
		fscanf(file, "%d%c",&(faces[nFaces].vIndices[i]), &tmp);
		faces[nFaces].vIndices[i]--;
		
		if(tmp == ' ')
		{
			faces[nFaces].tIndices[i] = -1;
			faces[nFaces].nIndices[i] = -1;
			continue;
		}
		else if(tmp == '/')
		{
			
			tmp = fgetc(file);
			if(tmp != '/' && tmp != ' ') {
				ungetc(tmp, file);
				fscanf(file, "%d%c",&(faces[nFaces].tIndices[i]), &tmp);
				faces[nFaces].tIndices[i]--;
			}
			else// if(tmp == '/')
			{
				faces[nFaces].tIndices[i] = -1;
			}
		}
		else	//Very bad error probably...nonsense .OBJ...polygon with less than 3 vertices
		{
			faces[nFaces].tIndices[i] = -1;
			faces[nFaces].nIndices[i] = -1;
			continue;
		}
		
		//tmp = fgetc(file);
		if(tmp == ' ')
		{
			faces[nFaces].nIndices[i] = -1;
			continue;
		}
		else if(tmp != ' ' && tmp != '\n')
		{
			//ungetc(tmp,file);
			fscanf(file, "%d%c",&(faces[nFaces].nIndices[i]), &tmp);
			faces[nFaces].nIndices[i]--;
		}
		//if(tmp == ' ' || tmp == '\n' || tmp == '/') faces[nFaces].nIndices[i] = -1;
		//tmp = fgetc(file);
		if(tmp != ' ' && tmp != '\n') ungetc(tmp,file);
	}
	//tmp = fgetc(file);
	//if(tmp != '\n') ungetc(tmp,file);
	
	strncpy(faces[nFaces].mtlName,currentMtlName, 24);
	
	nFaces++;
	if(nFaces >= EX_VERTICES)
	{
		realloc_mem<Face3>(nFaces, nFaces+1, &faces);
	}
}

void OBJPart::free()
{
	if(vertices)
	delete[] vertices;
	if(normals)
	delete[] normals;
	if(texcoords)
	delete[] texcoords;
	if(faces)
	delete[] faces;
	
	vertices = 0;
	normals = 0;
	texcoords = 0;
	faces = 0;
	
		for(int i=0;i<nMtlParts;i++)
		{
			if(array[i].vertices) delete[] array[i].vertices;
			if(array[i].normals) delete[] array[i].normals;
			if(array[i].texcoords) delete[] array[i].texcoords;
		}
}

void OBJPart::initArrays()
{
	int processing = 1, currentMtlPart = 0, oldI = 0;
	unsigned int oldMtl = 0, currentMtl = 0;
	char currentMtlName[25];
	memset(currentMtlName, 0, 25);
	//while(processing)
	//{
	
#define ALLOC_NEW_ARRAYS	array[currentMtlPart].count = nFaces*3-oldI*3;\
	array[currentMtlPart].vertices = new Point3[array[currentMtlPart].count/*-oldI*3*/];\
	array[currentMtlPart].normals = new Point3[array[currentMtlPart].count/*-oldI*3*/];\
	array[currentMtlPart].texcoords = new TexCoord2[array[currentMtlPart].count/*-oldI*3*/];\
	memset(array[currentMtlPart].vertices, 0, sizeof(Point3)*array[currentMtlPart].count);\
	memset(array[currentMtlPart].normals, 0, sizeof(Point3)*array[currentMtlPart].count);\
	memset(array[currentMtlPart].texcoords, 0, sizeof(TexCoord2)*array[currentMtlPart].count);
	ALLOC_NEW_ARRAYS
	Point3 blank = {0.0f, 0.0f, 0.0f};
	for(int i=oldI;i<nFaces&&processing;i++)
	{
		//oldMtl = currentMtl;
		//currentMtl = ((OBJFile*)parent)->mtllib.getMtlByName(faces[i].mtlName)->diffuse_texture;if(currentMtl != oldMtl && oldMtl)
		//fprintf(stderr, "Face %d, material %s, matname len %d\n", i, faces[i].mtlName, strlen(faces[i].mtlName));
		if(strncmp(faces[i].mtlName, currentMtlName, 24) && strlen(currentMtlName))
		{
			//oldMtl = currentMtl;
			strncpy(currentMtlName, faces[i].mtlName, 24);
			array[currentMtlPart].count = (i-oldI)*3;
			oldI = i;
			//fprintf(stderr, "New MTL part for material %s\n", currentMtlName);
			currentMtlPart++;
			if(currentMtlPart >= MAX_MTL_PARTS) {processing = 0;currentMtlPart--; continue;}
			strncpy(array[currentMtlPart].material, faces[i].mtlName, 24);
			
			ALLOC_NEW_ARRAYS
			//continue;
		}
		else if(!strlen(currentMtlName))
		{
			strncpy(array[currentMtlPart].material, faces[i].mtlName, 24);
			strncpy(currentMtlName, faces[i].mtlName, 24);
			array[currentMtlPart].count = i*3;
			//oldMtl = currentMtl;
		}
		
		for(int j=0;j<3;j++)
		{
			memcpy(&(array[currentMtlPart].vertices[(i-oldI)*3+j]), &(vertices[faces[i].vIndices[j]]), sizeof(Point3));
			//fprintf(stderr, "Vertex: %f %f %f\n", array.vertices[i*3+j].x, array.vertices[i*3+j].y, array.vertices[i*3+j].z);
			if(nNormals) memcpy(&(array[currentMtlPart].normals[(i-oldI)*3+j]), &(normals[faces[i].nIndices[j]]), sizeof(Point3));
			else memcpy(&(array[currentMtlPart].normals[(i-oldI)*3+j]), &blank, sizeof(Point3));
			if(nTexcoords) memcpy(&(array[currentMtlPart].texcoords[(i-oldI)*3+j]), &(texcoords[faces[i].tIndices[j]]), sizeof(TexCoord2));
			else memcpy(&(array[currentMtlPart].texcoords[(i-oldI)*3+j]), &blank, sizeof(TexCoord2));
		}
	}
	strcpy(array[currentMtlPart].material, faces[oldI].mtlName);
	nMtlParts = currentMtlPart+1;
	//array[currentMtlPart].count = i-oldI;
	processing = 0;
	//}
}

