/*
 wfmtl.cpp
 Routines for handling materials in a Wavefront OBJ file

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

#include <stdio.h>
#include <texture.h>
#include <string.h>
#include "wfmtl.h"
#ifndef USE_ES
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#else
#include <GLES/gl.h>
#endif

OBJMtlLib::OBJMtlLib()
{
	nMtls = 0;
	memset(materials, 0, sizeof(OBJMaterial)*MAX_MTLS);
	//printf("Cleared %d mtls.\n", MAX_MTLS);
	//int i;
	//for(i=0;i<MAX_MTLS;i++) materials[i].diffuse_texture = 0;
}
OBJMtlLib::~OBJMtlLib()
{
	for(int i = 0;i<nMtls;i++) {
		glDeleteTextures(1,&(materials[i].diffuse_texture));
		if(materials[i].id) delete materials[i].id;
	}
}
bool OBJMtlLib::load(const char *fileName)
{
	char tmp, tmps[100];//, tmps2[100];
	memset(tmps, 0, 100);
	if(relativePath)
	{
		strncpy(tmps, relativePath, 99);
	}
	strncat(tmps, fileName, 99-strlen(tmps));
	
	FILE *file = fopen(tmps, "r");
	if(file == NULL) { printf("OBJLOADER ERROR: Unable to load material library \'%s\'.\n",tmps);return false;}
	
	while(!feof(file) && nMtls < MAX_MTLS){
		tmp = fgetc(file);
		if(tmp == '#') fgets(tmps,99,file);
		else {
			ungetc(tmp,file);
			
			fscanf(file,"%99s",tmps);
			if(!strcmp("newmtl",tmps))
			{
				nMtls++;
				fgets(materials[nMtls-1].name,24,file);
	
				//fscanf(file, "%24s", materials[nMtls-1].name);
				tmp = fgetc(file);
				ungetc(tmp, file);
				if(tmp != '\n') fgets(tmps, 99, file);
				//fscanf(file, "%24s", materials[nMtls-1].name);
				//tmp = fgetc(file);
				
			}
			else if(!strcmp("Ka",tmps))
			{
				fscanf(file,"%f %f %f\n",&(materials[nMtls-1].ambient[0]),&(materials[nMtls-1].ambient[1]),&(materials[nMtls-1].ambient[2]));
				materials[nMtls-1].ambient[3] = 1.0f;
			}
			else if(!strcmp("Kd",tmps))
			{
				fscanf(file,"%f %f %f\n",&(materials[nMtls-1].diffuse[0]),&(materials[nMtls-1].diffuse[1]),&(materials[nMtls-1].diffuse[2]));
				materials[nMtls-1].diffuse[3] = 1.0f;
			}
			else if(!strcmp("Ks",tmps))
			{
				fscanf(file,"%f %f %f\n",&(materials[nMtls-1].specular[0]),&(materials[nMtls-1].specular[1]),&(materials[nMtls-1].specular[2]));
				materials[nMtls-1].specular[3] = 1.0f;
			}
			else if(!strcmp("map_Kd",tmps))
			{
				//fscanf(file, "%99s", materials[nMtls-1].diffuse_texture_fname);
				tmp = fgetc(file);
				if(tmp != ' ') ungetc(tmp, file);
				fgets(materials[nMtls-1].diffuse_texture_fname, 99, file);
				strncpy(tmps, relativePath, 99);
				strncat(tmps, materials[nMtls-1].diffuse_texture_fname, 99-strlen(tmps));
				strncpy(materials[nMtls-1].diffuse_texture_fname, tmps, 99);
				materials[nMtls-1].diffuse_texture_fname[strlen(materials[nMtls-1].diffuse_texture_fname)-1] = 0;
			#ifndef USE_PRELOADER
				materials[nMtls-1].diffuse_texture = loadTexture(materials[nMtls-1].diffuse_texture_fname);
				materials[nMtls-1].finished_loading = false;
			#else
				materials[nMtls-1].id = preloadTexture(materials[nMtls-1].diffuse_texture_fname);
			#endif
				#ifdef DEBUG_INFO
				if(materials[nMtls-1].diffuse_texture) printf("Loaded texture %s as texture object %d.\n", materials[nMtls-1].diffuse_texture_fname, materials[nMtls-1].diffuse_texture);
				else printf("Error loading %s.\n", materials[nMtls-1].diffuse_texture_fname);
				#endif
				materials[nMtls-1].has_texture = 1;
			}
			else
			{
				#ifdef DEBUG_INFO
				printf("Unknown/Unimplemented: %s.\n", tmps);
				#endif
				fgets(tmps,99,file);
			}
			//materials[nMtls-1].diffuse_texture = 0;
		}
	}
	
	fclose(file);
	return true;
}

OBJMaterial *OBJMtlLib::getMtlByName(const char *n)
{
	for(int i = 0;i<nMtls;i++)
	{
		if(strlen(n) != strlen(materials[i].name)) continue;
		else if(!strcmp(n,materials[i].name)) 
		{
		
		#ifdef USE_PRELOADER
			if(!materials[i].finished_loading) 
			{
				//printf("Finishing loading %s...\n", materials[i].diffuse_texture_fname);
				mtlFinishLoading( &(materials[i]) );
			}
		#endif
			return &(materials[i]);
		}
	}
	return NULL;
}

void mtlFinishLoading(OBJMaterial *mtl)
{
	unsigned int t;
	if(mtl->id) 
	{
		t = postloadTexture(mtl->id);
		if(!t) printf("Texture was supposed to finish loading, but it didn't.\n");
		mtl->diffuse_texture = mtl->id->tex;
	}
	//else printf("It didn't have an imagedata struct.\n");
	
	mtl->finished_loading = true;
}
