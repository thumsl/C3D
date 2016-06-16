#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"
#include "linmath.h"
// TODO: vertex struct

typedef struct {
	vec3 min;
	vec3 max;
} hitbox;

typedef struct {
	GLuint VAO;
	GLuint VBO; // POSITION BUFFER OBJECT
	GLuint EBO; // INDEX BUFFER OBJECT
	GLuint tex_id;
	unsigned int vertexCount;
	unsigned int indexCount;
	hitbox body;
} mesh;

mesh* initMesh(GLfloat *vertices, GLuint *indices, int vertexCount, int indexCount, const char* texLocation);
mesh* initOBJMesh(const char* filename, const char* texturePath);

static void setVertexData(mesh* M, GLfloat *vertices, const char* texturePath);
static void setMeshIndex(mesh* M, GLuint *indices);

int aabb_collision(hitbox a, hitbox b);

void draw(mesh *M);

// TODO: Clean up
#endif