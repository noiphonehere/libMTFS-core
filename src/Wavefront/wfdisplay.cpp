/*
 wfdisplay.cpp
 Routines for displaying a Wavefront OBJ file

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

#include "wfdisplay.h"

#ifndef USE_ES
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#else
#include <GLES/gl.h>
#endif

#include <stdio.h>

#ifndef USE_ES
void drawObj(OBJFile *of)
{
	#ifdef DEBUG_INFO_OBJLOADER
	static int first_time = 1;
	#endif
	static unsigned int curr_tex = 0;
	OBJMaterial *pmtl = 0;
	OBJMaterial *last_pmtl;
	glBegin(GL_TRIANGLES);	//Prepare to draw triangles
	for(int i = 0;i<of->nObjects;i++)
	{
		#ifdef DEBUG_INFO_OBJLOADER
		if(first_time) printf("Obj. %d; %d faces\n", i,of->parts[i].nFaces);
		#endif
		for(int j=0;j<of->parts[i].nFaces;j++)
		{
			for(int k=0;k<3;k++)
			{
				last_pmtl = pmtl;
				pmtl = of->mtllib.getMtlByName(of->parts[i].faces[j].mtlName);
				if(pmtl && pmtl != last_pmtl){
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pmtl->ambient);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pmtl->diffuse);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pmtl->specular);
					if(pmtl->has_texture) {
						//printf("Tex %d\n", pmtl->diffuse_texture);
						if(curr_tex != pmtl->diffuse_texture)
						{
							/*You can't switch texture objects in the middle of a glBegin/glEnd block...
							I learned that the hard way :(  */
							glEnd();
							#ifdef DEBUG_INFO_OBJLOADER
							if(first_time) 
							printf("Switching from texture object %d to %d at face %d of %d. (Material %s).\n", curr_tex, pmtl->diffuse_texture, j, of->parts[i].nFaces, of->parts[i].faces[j].mtlName);
							#endif
							if(!glIsEnabled(GL_TEXTURE_2D)) glEnable(GL_TEXTURE_2D);
							
							
							glBindTexture(GL_TEXTURE_2D, pmtl->diffuse_texture);
							curr_tex = pmtl->diffuse_texture;
							glBegin(GL_TRIANGLES);
							
						}
					}
					else// if(glIsEnabled(GL_TEXTURE_2D) == GL_TRUE) 
					{
						glEnd();
						#ifdef DEBUG_INFO_OBJLOADER
						if(first_time) 
						printf("Disabling texturing at face %d of %d (Material %s).\n", j, of->parts[i].nFaces, of->parts[i].faces[j].mtlName);
						#endif
						//glBindTexture(GL_TEXTURE_2D, 0);
						glDisable(GL_TEXTURE_2D);
						glBegin(GL_TRIANGLES);
					}
				}
				if(of->parts[i].nTexcoords && 
				of->parts[i].faces[j].tIndices[k] > -1 && of->parts[i].faces[j].tIndices[k] < of->parts[i].nTexcoords)
					glTexCoord2fv((GLfloat*)&(of->parts[i].texcoords[of->parts[i].faces[j].tIndices[k]]));
				if(of->parts[i].nNormals &&
				of->parts[i].faces[j].nIndices[k] > -1)
					glNormal3fv((GLfloat*)&(of->parts[i].normals[of->parts[i].faces[j].nIndices[k]]));
				if(of->parts[i].faces[j].vIndices[k] > of->parts[i].nVertices || of->parts[i].faces[j].vIndices[k] < 0) {printf("Part %d, Face %d, edge %d has invalid index %d!\n",i, j, k, of->parts[i].faces[j].vIndices[k]); continue;}
				glVertex3fv((GLfloat*)&(of->parts[i].vertices[of->parts[i].faces[j].vIndices[k]]));
			}
		}
	}
	glEnd();	//Finished drawing triangles
	#ifdef DEBUG_INFO_OBJLOADER
	first_time = 0;
	#endif
	curr_tex = 0;	//Reset the texture. We need that when something else messes up the texture...
}
#endif

void drawObjArrays(OBJFile *of)
{
	
	float mat[4] = {0.7f, 0.7f, 0.7f, 1.0f};
	OBJMaterial *pmtl;
	for(int i=0;i<of->parts[0].nMtlParts;i++)
	{
		pmtl = of->mtllib.getMtlByName(of->parts[0].array[i].material);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pmtl->diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pmtl->ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pmtl->specular);
		if(pmtl->has_texture)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, pmtl->diffuse_texture);
		}
		else glDisable(GL_TEXTURE_2D);
		
		glVertexPointer(3, GL_FLOAT, 0, of->parts[0].array[i].vertices);
		if(of->parts[0].nNormals)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, of->parts[0].array[i].normals);
		}
		else glDisableClientState(GL_NORMAL_ARRAY);
		if(of->parts[0].nTexcoords)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, of->parts[0].array[i].texcoords);
		}
		else glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDrawArrays(GL_TRIANGLES, 0, of->parts[0].array[i].count);
	}
}

