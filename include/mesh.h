#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"
#include "aabb.h"
#include "linmath.h"
#include "shader.h"

#include <stdbool.h>

// TODO: vertex struct

typedef struct {
	//quat orientation;
	mat4x4 translate;
	mat4x4 rotate;
	mat4x4 scale;
	mat4x4 model;
} transformMatrixes; // TODO: change all stuff like this to start with a capital letter

typedef struct {
	unsigned int vertexCount;
	unsigned int indexCount;
	GLuint VAO;
	GLuint VBO; // POSITION BUFFER OBJECT
	GLuint EBO; // INDEX BUFFER OBJECT
	GLuint textureID;
	transformMatrixes transform;
	bounding_box hitbox;
	GLuint hitboxVAO;
	GLuint hitboxVBO;
	GLuint hitboxEBO;
} mesh;

static void initMesh(mesh *M);
static void setVertexData(mesh* M, GLfloat *vertices, const char* texturePath);
static void setMeshIndex(mesh* M, GLuint *indices);

static void genHitboxVertexData(mesh* M);
void drawHitbox(mesh* M);

mesh* OBJToMesh(const char* filename, const char* texturePath);

void mesh_translate(mesh* M, float x, float y, float z);
void mesh_translate_from_origin(mesh* M, float x, float y, float z);
void mesh_rotate_x(mesh* M, float angle);
void mesh_rotate_y(mesh* M, float angle);
void mesh_rotate_z(mesh* M, float angle);
void mesh_rotate_from_ident(mesh* M, float x_angle, float y_angle, float z_angle);
void mesh_scale(mesh* M, float x, float y, float z);
void mesh_update_model_matrix(mesh* M);

void draw(mesh *M, mat4x4 view, mat4x4 projection, shader S, bool hitbox);

// TODO: Clean up
#endif