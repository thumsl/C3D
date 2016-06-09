#include "mesh.h"
#include <stdio.h>

static void setMeshIndex(mesh* M, GLuint *indices) {
	glBindVertexArray(M->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, M->indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

static void setMeshPosition(mesh *M, GLfloat *vertices) {
	glBindVertexArray(M->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, M->VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * M->vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);    //We like submitting vertices on stream 0 for no special reason
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);   //The starting point of the VBO, for the vertices

	glEnableVertexAttribArray(1);    //We like submitting vertices on stream 0 for no special reason
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

mesh* initMesh(GLfloat *vertices, GLuint *indices, int vertexCount, int indexCount) {
	mesh *M = (mesh*)malloc(sizeof(mesh));

	M->vertexCount = vertexCount;
	M->indexCount = indexCount;


	glGenVertexArrays(1, &(M->VAO));

	glGenBuffers(1, &(M->VBO));	
	setMeshPosition(M, vertices);

	glGenBuffers(1, &(M->IBO));
	setMeshIndex(M, indices);

	return M;
}

void draw(mesh *M) {
	glBindVertexArray(M->VAO);
	glDrawElements(GL_TRIANGLES, M->indexCount, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}