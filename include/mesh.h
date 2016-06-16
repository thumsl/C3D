#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"
#include "linmath.h"
// TODO: vertex struct

typedef struct {
	vec3 min;
	vec3 max;
} bounding_box;

typedef struct {
	mat4x4 translate;
	mat4x4 rotate;
	mat4x4 scale;
} transform_matrixes; // TODO: change all stuff like this to start with a capital letter

typedef struct {
	GLuint VAO;
	GLuint VBO; // POSITION BUFFER OBJECT
	GLuint EBO; // INDEX BUFFER OBJECT
	GLuint tex_id;
	unsigned int vertexCount;
	unsigned int indexCount;
	bounding_box hitbox;
	transform_matrixes transform;
} mesh;

mesh* initMesh(GLfloat *vertices, GLuint *indices, int vertexCount, int indexCount, const char* texLocation);
mesh* initOBJMesh(const char* filename, const char* texturePath);

static void setVertexData(mesh* M, GLfloat *vertices, const char* texturePath);
static void setMeshIndex(mesh* M, GLuint *indices);

int aabb_collision(bounding_box a, bounding_box b);

void draw(mesh *M);

// TODO: Clean up
#endif