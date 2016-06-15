#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

typedef struct {
	GLfloat* vertices;
	GLuint* indices;
	unsigned int vertexCount;
	unsigned int indexCount;
} OBJ_data;

float get_number(int* i, char* buffer, float* x);
int loadOBJ(OBJ_data* data, const char* filename);

#endif // OBJ_LOADER_H