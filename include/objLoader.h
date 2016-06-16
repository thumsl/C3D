	#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "mesh.h"

typedef struct {
	GLfloat* vertices;
	GLuint* indices;
	int vertexCount;
	int indexCount;
	bounding_box hitbox;
} OBJ_data;

float get_number(int* i, char* buffer, float* x);
int loadOBJ(OBJ_data** data, const char* filename);

#endif // OBJ_LOADER_H