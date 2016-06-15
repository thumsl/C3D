#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

float get_number(int* i, char* buffer, float* x);
int loadOBJ(const char* filename, GLfloat* vertices, GLuint* indices, int* vertexCount, int* indexCount);

#endif // OBJ_LOADER_H