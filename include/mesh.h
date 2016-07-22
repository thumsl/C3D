#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"
#include "aabb.h"
#include "linmath.h"
#include "list.h"
#include "material.h"
#include "shader.h"

#include <stdbool.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
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
	GLuint VBO;
	GLuint IBO;
	GLuint textureID;
	GLuint hitboxVAO;
	GLuint hitboxVBO;
	GLuint hitboxEBO;
	boundingBox hitbox;
	transformationMatrixes transform;
	material mat;
} mesh;

static void mesh_init(mesh *model);
static void mesh_loadToVAO(mesh* model, GLfloat* vertices, GLuint *indices);
static void mesh_textureFromFile(mesh *model, const char *texturePath);
static void mesh_setData(struct aiMesh* loadedMesh, mesh* model);
static void mesh_setMaterialData(struct aiMaterial* material, mesh* model, const char* texturePath);
void mesh_loadFromFileToList(const char* filename, const char* texturePath, linkedList* meshList);
mesh* mesh_loadFromFile(const char* filename, const char* texturePath);
mesh* mesh_genTerrain(const int terrainSize, const char *texturePath);
static void mesh_genHitboxMeshData(mesh* model);

void mesh_draw(mesh *model, mat4x4 view, mat4x4 projection, vec3* eyePos, shader S, bool hitbox);
void mesh_translate(mesh* model, float x, float y, float z);
void mesh_translate_from_origin(mesh* model, float x, float y, float z);
void mesh_rotate_x(mesh* model, float angle);
void mesh_rotate_y(mesh* model, float angle);
void mesh_rotate_z(mesh* model, float angle);
void mesh_rotate_from_ident(mesh* model, float x_angle, float y_angle, float z_angle);
void mesh_scale(mesh* model, float x, float y, float z);
void mesh_update_model_matrix(mesh* model);


// TODO: Clean up
#endif