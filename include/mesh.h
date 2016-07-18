#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"
#include "aabb.h"
#include "linmath.h"
#include "shader.h"

#include <stdbool.h>

// TODO: vertex struct

typedef struct {
	mat4x4 translate;
	mat4x4 rotate;
	mat4x4 scale;
	mat4x4 model;
} transformationMatrixes;

typedef struct {
	unsigned int vertexCount;
	unsigned int indexCount;
	GLuint VAO;
	GLuint VBO; // POSITION BUFFER OBJECT
	GLuint EBO; // INDEX BUFFER OBJECT
	GLuint textureID;
	transformationMatrixes transform;
	boundingBox hitbox;
	GLuint hitboxVAO;
	GLuint hitboxVBO;
	GLuint hitboxEBO;
} mesh;

static void mesh_init(mesh *model);
static void mesh_setIndexData(mesh* model, GLuint *indices);
static void mesh_setVertexData(mesh* model, GLfloat *vertices, const char* texturePath);

static void genHitboxVertexData(mesh* model);
void drawHitbox(mesh* model);

mesh* OBJToMesh(const char* filename, const char* texturePath);

void mesh_translate(mesh* model, float x, float y, float z);
void mesh_translate_from_origin(mesh* model, float x, float y, float z);
void mesh_rotate_x(mesh* model, float angle);
void mesh_rotate_y(mesh* model, float angle);
void mesh_rotate_z(mesh* model, float angle);
void mesh_rotate_from_ident(mesh* model, float x_angle, float y_angle, float z_angle);
void mesh_scale(mesh* model, float x, float y, float z);
void mesh_update_model_matrix(mesh* model);

void mesh_draw(mesh *model, mat4x4 view, mat4x4 projection, vec3* eyePos, shader S, bool hitbox);

// TODO: Clean up
#endif