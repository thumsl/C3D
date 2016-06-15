#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"

// TODO: vertex struct

typedef struct {
	GLuint VAO;
	GLuint VBO; // POSITION BUFFER OBJECT
	GLuint EBO; // INDEX BUFFER OBJECT
	GLuint tex_id;
	unsigned int vertexCount;
	unsigned int indexCount;
} mesh;

mesh* initMesh(GLfloat *vertices, GLuint *indices, int vertexCount, int indexCount, const char* texLocation);
mesh* initOBJMesh(const char* filename, const char* texturePath);

static void setVertexData(mesh* M, GLfloat *vertices, const char* texturePath);
static void setMeshIndex(mesh* M, GLuint *indices);

void draw(mesh *M);

// TODO: Clean up
#endif