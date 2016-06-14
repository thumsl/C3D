#include "../include/mesh.h"
#include "../include/utils.h" // needed?
#include <stdio.h>
#include "SDL2/SDL_image.h"

static void setMeshIndex(mesh* M, GLuint *indices) {
	glBindVertexArray(M->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, M->indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

static void setVertexData(mesh *M, GLfloat *vertices, const char* texLocation) {
	glBindVertexArray(M->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, M->VBO);
	glBufferData(GL_ARRAY_BUFFER, 5 * M->vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);    // Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);   //The starting point of the VBO, for the vertices

	glEnableVertexAttribArray(1);    // Texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

	SDL_Surface *image;
	image=IMG_Load(texLocation);
	if(!image) {
	    printf("IMG_Load: %s\n", IMG_GetError());
	}

	glBindTexture(GL_TEXTURE_2D, M->tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels); // vertices == image data

	SDL_FreeSurface(image);
}

mesh* initMesh(GLfloat *vertices, GLuint *indices, int vertexCount, int indexCount, const char* texLocation) {
	mesh *M = (mesh*)malloc(sizeof(mesh));

	M->vertexCount = vertexCount;
	M->indexCount = indexCount;

	glGenVertexArrays(1, &(M->VAO));

	glGenBuffers(1, &(M->VBO));
	glGenTextures(1, &(M->tex_id));
	setVertexData(M, vertices, texLocation);

	glGenBuffers(1, &(M->EBO));
	setMeshIndex(M, indices);

	return M;
}

void draw(mesh *M) {
	glBindVertexArray(M->VAO);
	glBindTexture(GL_TEXTURE_2D, M->tex_id);
	glDrawElements(GL_TRIANGLES, M->indexCount, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}