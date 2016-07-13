#include "../include/engine.h"
#include "../include/utils.h"
#include "../include/mesh.h"
#include "../include/objLoader.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

static void genHitboxVertexData(mesh* M) {	
	float position[] = {
		M->hitbox.min[0], M->hitbox.min[1], M->hitbox.min[2],
		M->hitbox.max[0], M->hitbox.min[1], M->hitbox.min[2],
		M->hitbox.max[0], M->hitbox.min[1], M->hitbox.max[2],
		M->hitbox.min[0], M->hitbox.min[1], M->hitbox.max[2],
		M->hitbox.min[0], M->hitbox.max[1], M->hitbox.max[2],
		M->hitbox.min[0], M->hitbox.max[1], M->hitbox.min[2],
		M->hitbox.max[0], M->hitbox.max[1], M->hitbox.min[2],
		M->hitbox.max[0], M->hitbox.max[1], M->hitbox.max[2]
    };

    GLuint indices[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 4,
        4, 5,
        5, 0,
        5, 6,
        6, 7,
        7, 2,
        6, 1,
        4, 7,
        3, 0
    };

	glGenVertexArrays(1, &(M->hitboxVAO));
	glGenBuffers(1, &(M->hitboxVBO));
	glGenBuffers(1, &(M->hitboxEBO));

	glBindVertexArray(M->hitboxVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M->hitboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, (M->hitboxVBO));
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

static void setMeshIndex(mesh* M, GLuint *indices) {
	glBindVertexArray(M->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, M->indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

static void setVertexData(mesh *M, GLfloat *vertices, const char* texLocation) {
	glBindVertexArray(M->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, M->VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * M->vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);    // Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);   //The starting point of the VBO, for the vertices

	glEnableVertexAttribArray(1);    // Texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);    // Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));

	SDL_Surface *image;
	image=IMG_Load(texLocation);
	if(!image) {
	    printf("IMG_Load: %s\n", IMG_GetError());
	}

	glBindTexture(GL_TEXTURE_2D, M->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels); // vertices == image data

	SDL_FreeSurface(image);
}

static void initMesh(mesh *M) {
	mat4x4_identity(M->transform.rotate);
	mat4x4_identity(M->transform.translate);
	mat4x4_identity(M->transform.scale);
}

mesh* OBJToMesh(const char* filename, const char* texturePath) {
	mesh *M = (mesh*)malloc(sizeof(mesh));

	initMesh(M);

	OBJ_data* data;
	loadOBJ(&data, filename); // TODO: Dynamic allocation for Mesh data arrays

	M->vertexCount = data->vertexCount;
	M->indexCount = data->indexCount;

	vec3_copy(M->hitbox.min, data->hitbox.min);
	vec3_copy(M->hitbox.max, data->hitbox.max);
	
	glGenVertexArrays(1, &(M->VAO));

	glGenBuffers(1, &(M->VBO));
	glGenTextures(1, &(M->textureID));
	setVertexData(M, data->vertices, texturePath);

	glGenBuffers(1, &(M->EBO));
	setMeshIndex(M, data->indices);

	genHitboxVertexData(M);

	free(data->indices);
	free(data->vertices);
	free(data);

	return M;
}

void draw(mesh *M, mat4x4 view, mat4x4 projection, shader S, bool hitbox) {
	mat4x4_mul(M->transform.model, M->transform.rotate, M->transform.scale);
	mat4x4_mul(M->transform.model, M->transform.translate, M->transform.model);

	mat4x4 model_view_projection;

	mat4x4_mul(model_view_projection, view, M->transform.model);
	mat4x4_mul(model_view_projection, projection, model_view_projection);

	glUniformMatrix4fv(S.location.MVP, 1, 0, (GLfloat*)model_view_projection);

	glBindVertexArray(M->VAO);
	glBindTexture(GL_TEXTURE_2D, M->textureID);
	glDrawElements(GL_TRIANGLES, M->indexCount, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);


	if (hitbox) {
		glBindVertexArray(M->hitboxVAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
}

void mesh_translate(mesh* M, float x, float y, float z) {
	mat4x4_translate(M->transform.translate, x, y, z);
	M->hitbox.min[0] += x;
	M->hitbox.min[1] += y;
	M->hitbox.min[2] += z;
	M->hitbox.max[0] += x;
	M->hitbox.max[1] += y;
	M->hitbox.max[2] += z;
}

void mesh_translate_from_origin(mesh* M, float x, float y, float z) {
	mat4x4_identity(M->transform.translate);
	mat4x4_translate(M->transform.translate, x, y, z);
	M->hitbox.min[0] += x;
	M->hitbox.min[1] += y;
	M->hitbox.min[2] += z;
	M->hitbox.max[0] += x;
	M->hitbox.max[1] += y;
	M->hitbox.max[2] += z;
}

// Use quaternions?
void mesh_rotate_x(mesh* M, float angle) {
	mat4x4_rotate(M->transform.rotate, M->transform.rotate, 1.0f, 0.0f, 0.0f, angle);
}

void mesh_rotate_y(mesh* M, float angle) {
	mat4x4_rotate(M->transform.rotate, M->transform.rotate, 0, 1, 0, angle);
}

void mesh_rotate_z(mesh* M, float angle) {
	mat4x4_rotate(M->transform.rotate, M->transform.rotate, 0, 0, 1, angle);
}

void mesh_rotate_from_ident(mesh* M, float x_angle, float y_angle, float z_angle) {
	mat4x4_identity(M->transform.rotate);
	mat4x4_rotate(M->transform.rotate, M->transform.rotate, 1, 0, 0, x_angle);
	mat4x4_rotate(M->transform.rotate, M->transform.rotate, 0, 1, 0, y_angle);
	mat4x4_rotate(M->transform.rotate, M->transform.rotate, 0, 0, 1, z_angle);
}

void mesh_scale(mesh* M, float x, float y, float z) {
	// M->transform.scale[0][0] *= k;
	// M->transform.scale[1][1] *= k;
	// M->transform.scale[2][2] *= k;
	mat4x4_scale_aniso(M->transform.scale, M->transform.scale, x, y, z);
}

void mesh_update_model_matrix(mesh* M) {
	mat4x4_mul(M->transform.model, M->transform.rotate, M->transform.scale);
	mat4x4_mul (M->transform.model, M->transform.translate, M->transform.model);
}