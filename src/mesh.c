#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "../include/c3d.h"
#include "../include/mesh.h"
#include "../include/phongShader.h"

void mesh_init(mesh *model)
{
	model->vertexCount = 0;
	model->indexCount = 0;
	model->mat.specularPower = 32;
	model->mat.specularIntensity = 1;
	model->hitbox = NULL;
	mat4x4_identity(model->matrices.rotate);
	mat4x4_identity(model->matrices.translate);
	mat4x4_identity(model->matrices.scale);

	glGenVertexArrays(1, &(model->VAO));
	glGenBuffers(1, &(model->VBO));
	glGenBuffers(1, &(model->IBO));
	glGenTextures(1, &(model->textureID));
}

void mesh_loadToVAO(mesh *model, GLfloat *vertices, GLuint *indices)
{
	// Compute the AABB from the vertex array (8 floats per vertex:
	// pos(3) + uv(2) + normal(3)). Without this, level_groundHeightAt
	// has nothing to query against and the player falls through the world.
	if (model->hitbox == NULL && model->vertexCount > 0) {
		model->hitbox = malloc(sizeof(boundingBox));
		model->hitbox->min[0] = INFINITY;
		model->hitbox->min[1] = INFINITY;
		model->hitbox->min[2] = INFINITY;
		model->hitbox->max[0] = -INFINITY;
		model->hitbox->max[1] = -INFINITY;
		model->hitbox->max[2] = -INFINITY;
		for (unsigned int i = 0; i < model->vertexCount; i++) {
			float x = vertices[i * 8 + 0];
			float y = vertices[i * 8 + 1];
			float z = vertices[i * 8 + 2];
			if (x < model->hitbox->min[0]) model->hitbox->min[0] = x;
			if (y < model->hitbox->min[1]) model->hitbox->min[1] = y;
			if (z < model->hitbox->min[2]) model->hitbox->min[2] = z;
			if (x > model->hitbox->max[0]) model->hitbox->max[0] = x;
			if (y > model->hitbox->max[1]) model->hitbox->max[1] = y;
			if (z > model->hitbox->max[2]) model->hitbox->max[2] = z;
		}
	}

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void mesh_textureFromFile(mesh *model, const char *texturePath)
{
	SDL_Surface *loaded;
	loaded = IMG_Load(texturePath);

	if (loaded == NULL) {
		fprintf(stderr, "Failed to load texture: %s\n", IMG_GetError());
		return;
	}

	// IMG_Load may return surfaces in any pixel format (RGB, RGBA, paletted,
	// etc). Convert to a known RGBA32 layout so the GL upload below can use
	// a single matching format and read the right number of bytes per pixel.
	SDL_Surface *image = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(loaded);
	if (image == NULL) {
		fprintf(stderr, "Failed to convert texture: %s\n", SDL_GetError());
		return;
	}

	// SDL surfaces are top-down; OpenGL textures are bottom-up. Flip rows in place.
	char *pixels = (char *)image->pixels;
	char *tmp = malloc(image->pitch);
	for (int y = 0; y < image->h / 2; y++) {
		char *row1 = pixels + y * image->pitch;
		char *row2 = pixels + (image->h - 1 - y) * image->pitch;
		memcpy(tmp, row1, image->pitch);
		memcpy(row1, row2, image->pitch);
		memcpy(row2, tmp, image->pitch);
	}
	free(tmp);

	glBindTexture(GL_TEXTURE_2D, model->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

	if (c3d_get_texture_quality() == C3D_TEXTURE_QUALITY_HIGH) {
		// Trilinear filtering between mip levels removes the shimmering
		// you get when distant texels alias against on-screen pixels.
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Anisotropic filtering sharpens textures viewed at grazing angles
		// (terrain near the horizon, long walls). Cap at 8x — past that the
		// quality gain is invisible. Skip silently if the extension isn't
		// present (older drivers/GPUs); the mipmap path alone is still a
		// big win.
		if (GLEW_EXT_texture_filter_anisotropic) {
			GLfloat max_aniso = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
			GLfloat aniso = max_aniso < 8.0f ? max_aniso : 8.0f;
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
		}
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	SDL_FreeSurface(image);
}

static void mesh_setData(struct aiMesh *loadedMesh, mesh *model)
{
	model->indexCount = loadedMesh->mNumFaces * 3;
	model->vertexCount = loadedMesh->mNumVertices;

	GLfloat *vertices = malloc(sizeof(GLfloat) * loadedMesh->mNumVertices * 8);
	GLuint *indices = malloc(sizeof(GLuint) * model->indexCount);

	model->hitbox = malloc(sizeof(boundingBox));

	model->hitbox->min[0] = INFINITY;
	model->hitbox->min[1] = INFINITY;
	model->hitbox->min[2] = INFINITY;
	model->hitbox->max[0] = -INFINITY;
	model->hitbox->max[1] = -INFINITY;
	model->hitbox->max[2] = -INFINITY;

	int i, j;
	for (i = 0, j = 0; j < loadedMesh->mNumVertices; i += 8, j++) {
		vertices[i] = loadedMesh->mVertices[j].x;
		if (loadedMesh->mVertices[j].x < model->hitbox->min[0]) {
			model->hitbox->min[0] = loadedMesh->mVertices[j].x;
		}
		if (loadedMesh->mVertices[j].x > model->hitbox->max[0])
			model->hitbox->max[0] = loadedMesh->mVertices[j].x;

		vertices[i + 1] = loadedMesh->mVertices[j].y;
		if (loadedMesh->mVertices[j].y < model->hitbox->min[1])
			model->hitbox->min[1] = loadedMesh->mVertices[j].y;
		if (loadedMesh->mVertices[j].y > model->hitbox->max[1])
			model->hitbox->max[1] = loadedMesh->mVertices[j].y;

		vertices[i + 2] = loadedMesh->mVertices[j].z;
		if (loadedMesh->mVertices[j].z < model->hitbox->min[2])
			model->hitbox->min[2] = loadedMesh->mVertices[j].z;
		if (loadedMesh->mVertices[j].z > model->hitbox->max[2])
			model->hitbox->max[2] = loadedMesh->mVertices[j].z;
	}

	if (loadedMesh->mTextureCoords[0] != NULL) {
		for (i = 0, j = 0; j < loadedMesh->mNumVertices; i += 8, j++) {
			vertices[i + 3] = loadedMesh->mTextureCoords[0][j].x;
			vertices[i + 4] = loadedMesh->mTextureCoords[0][j].y;
		}
	}

	if (loadedMesh->mNormals != NULL)
		for (i = 0, j = 0; j < loadedMesh->mNumVertices; i += 8, j++) {
			vertices[i + 5] = loadedMesh->mNormals[j].x;
			vertices[i + 6] = loadedMesh->mNormals[j].y;
			vertices[i + 7] = loadedMesh->mNormals[j].z;
		}

	for (i = 0, j = 0; j < loadedMesh->mNumFaces; i += 3, j++) {
		indices[i] = loadedMesh->mFaces[j].mIndices[0];
		indices[i + 1] = loadedMesh->mFaces[j].mIndices[1];
		indices[i + 2] = loadedMesh->mFaces[j].mIndices[2];
	}

	mesh_loadToVAO(model, vertices, indices);

	free(vertices);
	free(indices);
}

static void mesh_setMaterialData(struct aiMaterial *mat, mesh *model, const char *texturePath)
{
	unsigned int max = 1;
	float x;
	if (aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &x, &max) == AI_SUCCESS)
		model->mat.specularPower = x;
	if (aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS_STRENGTH, &x, &max) == AI_SUCCESS)
		model->mat.specularIntensity = x;

	printf("POWER: %.2f, INTENSITY %.2f\n", model->mat.specularPower, model->mat.specularIntensity);

	struct aiString path;

	SDL_Surface *image;
	if (texturePath != NULL)
		mesh_textureFromFile(model, texturePath);
	else if (aiGetMaterialString(mat, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), &path) == AI_SUCCESS) {
		char filename[9 + path.length];
		char r[] = "res/obj/";

		int i;
		for (i = 0; i < 8 + path.length; i++) {
			if (i < 8)
				filename[i] = r[i];
			else
				filename[i] = path.data[i - 8];
		}
		filename[i] = 0;

		mesh_textureFromFile(model, filename);
	} else
		return;
}

bool mesh_loadFromFileToList(const char *filename, const char *texturePath, linkedList *meshList)
{
	const struct aiScene *scene = aiImportFile(filename, aiProcess_Triangulate);

	if (!scene) {
		fprintf(stderr, "File \"%s\" does not exist or this program does \
not have permission to open it.\n",
			filename);
		c3d_quit();
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < scene->mNumMeshes; i++) {
		mesh *model = malloc(sizeof(mesh));
		mesh_init(model);
		mesh_setData(scene->mMeshes[i], model);
		mesh_setMaterialData(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], model, texturePath);
		mesh_genHitboxMeshData(model);
		list_insert(meshList, model);
	}

	aiReleaseImport(scene);
	return true;
}

mesh *mesh_loadFromFile(const char *filename, const char *texturePath)
{
	const struct aiScene *scene = aiImportFile(filename, aiProcess_Triangulate);
	if (!scene) {
		fprintf(stderr, "File \"%s\" does not exist or this program does \
not have permission to open it.\n",
			filename);
		c3d_quit();
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < scene->mNumMeshes; i++) {
		mesh *model = malloc(sizeof(mesh));
		mesh_init(model);
		mesh_setData(scene->mMeshes[i], model);
		mesh_setMaterialData(scene->mMaterials[0], model, texturePath);
		mesh_genHitboxMeshData(model);
		aiReleaseImport(scene);
		return model;
	}
	aiReleaseImport(scene);
	return NULL;
}

mesh *mesh_genFlatFloor(int size, const char *texturePath)
{
	if (size <= 0)
		return NULL;

	int i, j, k;
	mesh *model = malloc(sizeof(mesh));

	mesh_init(model);
	model->indexCount = 6 * (size - 1) * (size - 1);
	model->vertexCount = size * size;

	GLfloat *vertices = malloc(sizeof(GLfloat) * model->vertexCount * 8);
	GLuint *indices = malloc(sizeof(GLuint) * model->indexCount);

	for (i = 0, k = 0; i < size; i++)
		for (j = 0; j < size; j++, k += 8) {
			vertices[k] = i;
			vertices[k + 1] = 0;
			vertices[k + 2] = -j;
			vertices[k + 3] = (float)((int)j % 2);
			vertices[k + 4] = (float)((int)i % 2);
			vertices[k + 5] = 0;
			vertices[k + 6] = 1;
			vertices[k + 7] = 0;
		}

	for (i = 0, k = 0; i < size - 1; i++) {
		for (j = 0; j < size - 1; j++, k += 6) {
			indices[k] = i + size * j;
			indices[k + 1] = i + size + 1 + size * j;
			indices[k + 2] = i + 1 + size * j;
		}
	}

	for (i = 0, k = 3; i < size - 1; i++) {
		for (j = 0; j < size - 1; j++, k += 6) {
			indices[k] = i + size * j;
			indices[k + 1] = size + i + size * j;
			indices[k + 2] = i + size + 1 + size * j;
		}
	}

	mesh_loadToVAO(model, vertices, indices);
	mesh_textureFromFile(model, texturePath);

	free(indices);
	free(vertices);

	return model;
}

static void mesh_genHitboxMeshData(mesh *model)
{
	float position[] = { model->hitbox->min[0], model->hitbox->min[1], model->hitbox->min[2], model->hitbox->max[0], model->hitbox->min[1],
			     model->hitbox->min[2], model->hitbox->max[0], model->hitbox->min[1], model->hitbox->max[2], model->hitbox->min[0],
			     model->hitbox->min[1], model->hitbox->max[2], model->hitbox->min[0], model->hitbox->max[1], model->hitbox->max[2],
			     model->hitbox->min[0], model->hitbox->max[1], model->hitbox->min[2], model->hitbox->max[0], model->hitbox->max[1],
			     model->hitbox->min[2], model->hitbox->max[0], model->hitbox->max[1], model->hitbox->max[2] };

	GLuint indices[] = { 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 0, 5, 6, 6, 7, 7, 2, 6, 1, 4, 7, 3, 0 };

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

void mesh_draw(mesh *model, shader *S, camera *C, mat4x4 projection)
{
	switch (S->type) {
	case PHONG:
		shader_use(S);
		mesh_updateModelMatrix(model);
		phongShader_updateUniforms(model, S, C, projection);
		break;
	case TEXT:
		// text_draw() does all the work with uniforms before calling mesh_draw()
		// TODO: create a new "mesh" type for text and do all rendering inside text_draw()
		break;
	}

	glBindVertexArray(model->VAO);
	glBindTexture(GL_TEXTURE_2D, model->textureID);
	glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_INT, (void *)0);
	glBindVertexArray(0);
}

void mesh_drawList(linkedList *list, shader *S, camera *C, mat4x4 projection)
{
	node *aux = list->head;

	while (aux != NULL) {
		mesh_draw((mesh *)aux->data, S, C, projection);
		aux = aux->next;
	}
}

void mesh_translate(mesh *model, float x, float y, float z)
{
	mat4x4_translate(model->matrices.translate, x, y, z);

	if (model->hitbox != NULL) {
		model->hitbox->min[0] += x;
		model->hitbox->min[1] += y;
		model->hitbox->min[2] += z;
		model->hitbox->max[0] += x;
		model->hitbox->max[1] += y;
		model->hitbox->max[2] += z;
	}
}

void mesh_translate_from_origin(mesh *model, float x, float y, float z)
{
	if (model->hitbox != NULL) {
		float prev_x = model->matrices.translate[3][0];
		float prev_y = model->matrices.translate[3][1];
		float prev_z = model->matrices.translate[3][2];
		float dx = x - prev_x;
		float dy = y - prev_y;
		float dz = z - prev_z;
		model->hitbox->min[0] += dx;
		model->hitbox->min[1] += dy;
		model->hitbox->min[2] += dz;
		model->hitbox->max[0] += dx;
		model->hitbox->max[1] += dy;
		model->hitbox->max[2] += dz;
	}
	mat4x4_identity(model->matrices.translate);
	mat4x4_translate(model->matrices.translate, x, y, z);
}

// TODO: Use quaternions?
void mesh_rotate_x(mesh *model, float angle)
{
	mat4x4_rotate(model->matrices.rotate, model->matrices.rotate, 1.0f, 0.0f, 0.0f, angle);
}

void mesh_rotate_y(mesh *model, float angle)
{
	mat4x4_rotate(model->matrices.rotate, model->matrices.rotate, 0, 1, 0, angle);
}

void mesh_rotate_z(mesh *model, float angle)
{
	mat4x4_rotate(model->matrices.rotate, model->matrices.rotate, 0, 0, 1, angle);
}

void mesh_rotate_from_ident(mesh *model, float x_angle, float y_angle, float z_angle)
{
	mat4x4_identity(model->matrices.rotate);
	mat4x4_rotate(model->matrices.rotate, model->matrices.rotate, 1, 0, 0, x_angle);
	mat4x4_rotate(model->matrices.rotate, model->matrices.rotate, 0, 1, 0, y_angle);
	mat4x4_rotate(model->matrices.rotate, model->matrices.rotate, 0, 0, 1, z_angle);
}

void mesh_scale(mesh *model, float x, float y, float z)
{
	mat4x4_scale_aniso(model->matrices.scale, model->matrices.scale, x, y, z);
}

void mesh_updateModelMatrix(mesh *model)
{
	mat4x4_mul(model->matrices.transform, model->matrices.rotate, model->matrices.scale);
	mat4x4_mul(model->matrices.transform, model->matrices.translate, model->matrices.transform);
}
