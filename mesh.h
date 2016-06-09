#ifndef MESH_H
#define MESH_H

#include <stdlib.h>
#include "GL/glew.h"

typedef struct {
	GLuint VAO;
	GLuint VBO; // VERTEX BUFFER OBJECT
	GLuint IBO; // INDEX BUFFER OBJECT
	// int *vertex; (no need to store it in memory!!)
	unsigned int vertexCount;
	unsigned int indexCount;
	// int *color;
	// int *index;
} mesh;

mesh* initMesh(GLfloat *vertices, GLuint *indices, int vertexCount, int indexCount);

static void setMeshPosition(mesh* M, GLfloat *vertices);
static void setMeshIndex(mesh* M, GLuint *indices);

void draw(mesh *M);

#endif