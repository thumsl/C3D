#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"
#include "linmath.h"
#include "aabb.h"
// TODO: vertex struct

typedef struct {
	quat orientation;
	mat4x4 translate;
	mat4x4 rotate;
	mat4x4 scale;
	mat4x4 model;
} transform_type; // TODO: change all stuff like this to start with a capital letter

typedef struct {
	GLuint VAO;
	GLuint VBO; // POSITION BUFFER OBJECT
	GLuint EBO; // INDEX BUFFER OBJECT
	GLuint tex_id;
	unsigned int vertexCount;
	unsigned int indexCount;
	bounding_box hitbox;
	transform_type transform;
} mesh;

static void initMesh(mesh *M);
static void setVertexData(mesh* M, GLfloat *vertices, const char* texturePath);
static void setMeshIndex(mesh* M, GLuint *indices);

mesh* initOBJMesh(const char* filename, const char* texturePath);

void mesh_translate(mesh* M, float x, float y, float z);
void mesh_translate_from_origin(mesh* M, float x, float y, float z);
void mesh_rotate_x(mesh* M, float angle);
void mesh_rotate_y(mesh* M, float angle);
void mesh_rotate_z(mesh* M, float angle);
void mesh_rotate_from_ident(mesh* M, float x_angle, float y_angle, float z_angle);
void mesh_scale(mesh* M, float x, float y, float z);

void mesh_update_model_matrix(mesh* M);

void draw(mesh *M);

// TODO: Clean up
#endif