#include "../include/mesh.h"
#include "../include/engine.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void mesh_init(mesh *model) {
	model->vertexCount = 0;
	model->indexCount = 0;
	model->mat.specularPower = 16;
	model->mat.specularIntensity = 8;
    model->hitbox.min[0] =  INFINITY; model->hitbox.min[1] =  INFINITY; model->hitbox.min[2] =  INFINITY;
    model->hitbox.max[0] = -INFINITY; model->hitbox.max[1] = -INFINITY; model->hitbox.max[2] = -INFINITY;
	mat4x4_identity(model->transform.rotate);
	mat4x4_identity(model->transform.translate);
	mat4x4_identity(model->transform.scale);

    glGenVertexArrays(1, &(model->VAO));
    glGenBuffers(1, &(model->VBO));
    glGenBuffers(1, &(model->IBO));
    glGenTextures(1, &(model->textureID));
}

static void mesh_loadToVAO(mesh* model, GLfloat* vertices, GLuint *indices) {
	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * model->vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	
	// POSITION COORDINATES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	// TEXTURE COORDINATES
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// NORMALS
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);		

	// INDICES
	glGenBuffers(1, &(model->IBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void mesh_textureFromFile(mesh *model, const char *texturePath) {
	SDL_Surface *image;
	image = IMG_Load(texturePath);

	if(!image) {
	    fprintf(stderr, "Failed to load texture: %s\n", IMG_GetError());
	    return;
	}

	glBindVertexArray(model->VAO);
	glBindTexture(GL_TEXTURE_2D, model->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	SDL_FreeSurface(image);
}

static void mesh_setData(struct aiMesh* loadedMesh, mesh* model) {
	model->indexCount = loadedMesh->mNumFaces * 3;
	model->vertexCount = loadedMesh->mNumVertices;

	GLfloat vertices[loadedMesh->mNumVertices * 8];
	GLuint indices[model->indexCount]; // use malloc!
	int i, j;

	for (i = 0, j = 0; j < loadedMesh->mNumVertices; i += 8, j++) {
		vertices[i]   = loadedMesh->mVertices[j].x;
		if (loadedMesh->mVertices[j].x < model->hitbox.min[0]) {
			model->hitbox.min[0] = loadedMesh->mVertices[j].x;
		}
		else if (loadedMesh->mVertices[j].x > model->hitbox.max[0])
			model->hitbox.max[0] = loadedMesh->mVertices[j].x;

		vertices[i+1] = loadedMesh->mVertices[j].y;
		if (loadedMesh->mVertices[j].y < model->hitbox.min[1])
			model->hitbox.min[1] = loadedMesh->mVertices[j].y;
		else if (loadedMesh->mVertices[j].y > model->hitbox.max[1])
			model->hitbox.max[1] = loadedMesh->mVertices[j].y;

		vertices[i+2] = loadedMesh->mVertices[j].z;
		if (loadedMesh->mVertices[j].z < model->hitbox.min[2])
			model->hitbox.min[2] = loadedMesh->mVertices[j].z;
		else if (loadedMesh->mVertices[j].z > model->hitbox.max[2])
			model->hitbox.max[2] = loadedMesh->mVertices[j].z;
	}

	if (loadedMesh->mTextureCoords[0] != NULL) {
		for (i = 0, j = 0; j < loadedMesh->mNumVertices; i += 8, j++) {
			vertices[i+3] = loadedMesh->mTextureCoords[0][j].x;
			vertices[i+4] = loadedMesh->mTextureCoords[0][j].y;
		}
	}

	if (loadedMesh->mNormals!=NULL)
		for (i = 0, j = 0; j < loadedMesh->mNumVertices; i += 8, j++) {
			vertices[i+5] = loadedMesh->mNormals[j].x;
			vertices[i+6] = loadedMesh->mNormals[j].y;
			vertices[i+7] = loadedMesh->mNormals[j].z;
		}

	for (i = 0, j = 0; j < loadedMesh->mNumFaces; i+=3, j++) {
		indices[i] 	 = loadedMesh->mFaces[j].mIndices[0];
		indices[i+1] = loadedMesh->mFaces[j].mIndices[1];
		indices[i+2] = loadedMesh->mFaces[j].mIndices[2];
	}

	mesh_loadToVAO(model, vertices, indices);
}

static void mesh_setMaterialData(struct aiMaterial* mat, mesh* model, const char* texturePath) {
	// if (aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &(model->mat.specularPower), NULL) == AI_SUCCESS)
	// 	printf("Power changed: %f\n", model->mat.specularPower);
	// if (aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS_STRENGTH, &(model->mat.specularIntensity), NULL) == AI_SUCCESS)
	// 	printf("Intensity changed: %f\n", model->mat.specularIntensity);

	struct aiString path;
	
	SDL_Surface *image;
	if (texturePath != NULL)
		mesh_textureFromFile(model, texturePath);
	else if (aiGetMaterialString(mat, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), &path) == AI_SUCCESS) {
		char filename[9+path.length];
		char r[] = "res/obj/";

		int i;
		for (i = 0; i < 8 + path.length; i++) {
			if (i < 8)
				filename[i] = r[i];
			else
				filename[i] = path.data[i-8];
		}
		filename[i] = 0;

		mesh_textureFromFile(model, texturePath);
	}
	else
		return;
}

void mesh_loadFromFileToList(const char* filename, const char* texturePath, linkedList* meshList) {
	const struct aiScene* scene = aiImportFile(filename, aiProcess_Triangulate);
	if (!scene) {
		fprintf(stderr, "%s\n", aiGetErrorString());
		return;
	}

	int i;
	for (i = 0; i < scene->mNumMeshes; i++) {
		mesh* model = (mesh*)malloc(sizeof(mesh));
		mesh_init(model);
		mesh_setData(scene->mMeshes[i], model);
		mesh_setMaterialData(scene->mMaterials[0], model, texturePath);
		mesh_genHitboxMeshData(model);
		list_insert(meshList, model);
	}
}

mesh* mesh_loadFromFile(const char* filename, const char* texturePath) {
	const struct aiScene* scene = aiImportFile(filename, aiProcess_Triangulate);
	if (!scene) {
		fprintf(stderr, "%s\n", aiGetErrorString());
		return NULL;
	}

	int i;
	for (i = 0; i < scene->mNumMeshes; i++) {
		mesh* model = (mesh*)malloc(sizeof(mesh));
		mesh_init(model);
		mesh_setData(scene->mMeshes[i], model);
		mesh_setMaterialData(scene->mMaterials[0], model, texturePath);
		mesh_genHitboxMeshData(model);
		return model;
	}
}

mesh* mesh_genTerrain(const int terrainSize, const char *texturePath) {
	if (terrainSize <= 0)
		return NULL;

	int i, j, k;
	mesh* model = (mesh*)malloc(sizeof(mesh));
	mesh_init(model);

	model->indexCount = 6 * (terrainSize - 1) * (terrainSize - 1);
	model->vertexCount = terrainSize * terrainSize;

	GLfloat *vertices = (GLfloat*)malloc(sizeof(GLfloat) * model->vertexCount * 8);
	GLuint *indices = (GLuint*)malloc(sizeof(GLuint) * model->indexCount);

	for (i = 0, k = 0; i < terrainSize; i++)
		for (j = 0; j < terrainSize; j++, k+=8) {
			vertices[k] = -terrainSize/2 + i;
			vertices[k+1] = rand() % TERRAIN_MAXHEIGHT;
			vertices[k+2] = terrainSize/2 - j;
			vertices[k+3] = (float)((int)j % 2);
			vertices[k+4] = (float)((int)i % 2);
			vertices[k+5] = vertices[k+1] * 1.0f/TERRAIN_MAXHEIGHT;
			vertices[k+6] = vertices[k+1] * -1.0f/TERRAIN_MAXHEIGHT + 1;
			vertices[k+7] = vertices[k+1] * 1.0f/TERRAIN_MAXHEIGHT;
		}

	for (i = 0, k = 0; i < terrainSize - 1; i++) {
		for (j = 0; j < terrainSize - 1; j++, k+=6) {
			indices[k] = i + terrainSize*j;
			indices[k+1] = i + terrainSize + 1 + terrainSize*j;
			indices[k+2] = i + 1 + terrainSize*j;
		}
	}

	for (i = 0, k = 3; i < terrainSize - 1; i++) {
		for (j = 0; j < terrainSize - 1; j++, k+=6) {
			indices[k] = i + terrainSize*j;
			indices[k+1] = terrainSize + i + terrainSize*j;
			indices[k+2] = i + terrainSize + 1 + terrainSize*j;
		}
	}

	mesh_loadToVAO(model, vertices, indices);
	mesh_textureFromFile(model, texturePath);

	free(indices);
	free(vertices);
	return model;
}

static void mesh_genHitboxMeshData(mesh* model) {	
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