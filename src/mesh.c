#include "../include/engine.h"
#include "../include/utils.h"
#include "../include/mesh.h"
#include "../include/objLoader.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

static void mesh_init(mesh *model) {
	mat4x4_identity(model->transform.rotate);
	mat4x4_identity(model->transform.translate);
	mat4x4_identity(model->transform.scale);

	model->mat.specularPower = 16;
	model->mat.specularIntensity = 2;
}

static void mesh_setIndexData(mesh* model, GLuint *indices) {
	glBindVertexArray(model->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

static void mesh_setVertexData(mesh *model, GLfloat *vertices, const char* texLocation) {
	glBindVertexArray(model->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * model->vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

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

	glBindTexture(GL_TEXTURE_2D, model->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels); // vertices == image data

	SDL_FreeSurface(image);
}

mesh* OBJToMesh(const char* filename, const char* texturePath) {
	mesh *model = (mesh*)malloc(sizeof(mesh));

	mesh_init(model);

	OBJ_data* data;
	OBJ_load(&data, filename); // TODO: Dynamic allocation for Mesh data arrays

	model->vertexCount = data->vertexCount;
	model->indexCount = data->indexCount;

	vec3_copy(model->hitbox.min, data->hitbox.min);
	vec3_copy(model->hitbox.max, data->hitbox.max);
	
	glGenVertexArrays(1, &(model->VAO));
	glGenBuffers(1, &(model->EBO));
	glGenBuffers(1, &(model->VBO));
	glGenTextures(1, &(model->textureID));

	mesh_setIndexData(model, data->indices);
	mesh_setVertexData(model, data->vertices, texturePath);

	genHitboxVertexData(model);

	free(data->indices);
	free(data->vertices);
	free(data);

	return model;
}

void mesh_draw(mesh *model, mat4x4 view, mat4x4 projection, vec3* eyePos, shader S, bool hitbox) {
	mat4x4_mul(model->transform.model, model->transform.rotate, model->transform.scale);
	mat4x4_mul(model->transform.model, model->transform.translate, model->transform.model);

	mat4x4 model_view_projection;

	mat4x4_mul(model_view_projection, view, model->transform.model);
	mat4x4_mul(model_view_projection, projection, model_view_projection);

	glUniformMatrix4fv(S.location.MVP, 1, 0, (GLfloat*)model_view_projection);
	glUniformMatrix4fv(S.location.Transform, 1, 0, (GLfloat*)model->transform.model);
	glUniform3fv(S.location.eyePos, 1, (GLfloat*)eyePos);
	glUniform1f(S.location.specularPower, model->mat.specularPower);
	glUniform1f(S.location.specularIntensity, model->mat.specularIntensity);

	glBindVertexArray(model->VAO);
	glBindTexture(GL_TEXTURE_2D, model->textureID);
	glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);


	if (hitbox) {
		glBindVertexArray(model->hitboxVAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
}

void mesh_translate(mesh* model, float x, float y, float z) {
	mat4x4_translate(model->transform.translate, x, y, z);
	model->hitbox.min[0] += x;
	model->hitbox.min[1] += y;
	model->hitbox.min[2] += z;
	model->hitbox.max[0] += x;
	model->hitbox.max[1] += y;
	model->hitbox.max[2] += z;
}

void mesh_translate_from_origin(mesh* model, float x, float y, float z) {
	mat4x4_identity(model->transform.translate);
	mat4x4_translate(model->transform.translate, x, y, z); // TODO: hitbox needs to be reset first
	model->hitbox.min[0] += x; // this doesnt work
	model->hitbox.min[1] += y;
	model->hitbox.min[2] += z;
	model->hitbox.max[0] += x;
	model->hitbox.max[1] += y;
	model->hitbox.max[2] += z;
}

// Use quaternions?
void mesh_rotate_x(mesh* model, float angle) {
	mat4x4_rotate(model->transform.rotate, model->transform.rotate, 1.0f, 0.0f, 0.0f, angle);
}

void mesh_rotate_y(mesh* model, float angle) {
	mat4x4_rotate(model->transform.rotate, model->transform.rotate, 0, 1, 0, angle);
}

void mesh_rotate_z(mesh* model, float angle) {
	mat4x4_rotate(model->transform.rotate, model->transform.rotate, 0, 0, 1, angle);
}

void mesh_rotate_from_ident(mesh* model, float x_angle, float y_angle, float z_angle) {
	mat4x4_identity(model->transform.rotate);
	mat4x4_rotate(model->transform.rotate, model->transform.rotate, 1, 0, 0, x_angle);
	mat4x4_rotate(model->transform.rotate, model->transform.rotate, 0, 1, 0, y_angle);
	mat4x4_rotate(model->transform.rotate, model->transform.rotate, 0, 0, 1, z_angle);
}

void mesh_scale(mesh* model, float x, float y, float z) {
	// model->transform.scale[0][0] *= k;
	// model->transform.scale[1][1] *= k;
	// model->transform.scale[2][2] *= k;
	mat4x4_scale_aniso(model->transform.scale, model->transform.scale, x, y, z);
}

void mesh_update_model_matrix(mesh* model) {
	mat4x4_mul(model->transform.model, model->transform.rotate, model->transform.scale);
	mat4x4_mul (model->transform.model, model->transform.translate, model->transform.model);
}

static void genHitboxVertexData(mesh* model) {	
	float position[] = {
		model->hitbox.min[0], model->hitbox.min[1], model->hitbox.min[2],
		model->hitbox.max[0], model->hitbox.min[1], model->hitbox.min[2],
		model->hitbox.max[0], model->hitbox.min[1], model->hitbox.max[2],
		model->hitbox.min[0], model->hitbox.min[1], model->hitbox.max[2],
		model->hitbox.min[0], model->hitbox.max[1], model->hitbox.max[2],
		model->hitbox.min[0], model->hitbox.max[1], model->hitbox.min[2],
		model->hitbox.max[0], model->hitbox.max[1], model->hitbox.min[2],
		model->hitbox.max[0], model->hitbox.max[1], model->hitbox.max[2]
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

	glGenVertexArrays(1, &(model->hitboxVAO));
	glGenBuffers(1, &(model->hitboxVBO));
	glGenBuffers(1, &(model->hitboxEBO));

	glBindVertexArray(model->hitboxVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->hitboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, (model->hitboxVBO));
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
