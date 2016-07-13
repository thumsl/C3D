#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "mesh.h"

typedef struct {
	GLfloat* vertices;
	GLuint* indices;
	unsigned int vertexCount;
	unsigned int indexCount;
	boundingBox hitbox;
} OBJ_data;

struct vertex_info {
    int normalIndex;
    int textureIndex;
};

static float get_number(int* i, char* buffer, float* x);
void get_face_info(int* i, char* buffer, GLuint* f_temp, int *f, struct vertex_info* info);

int OBJ_load(OBJ_data** data, const char* filename);


#endif // OBJ_LOADER_H