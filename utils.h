#ifndef UTILS_H
#define UTILS_H
#include "linmath.h"
#include <GL/glew.h>

#define toRadian(i) i*0.0174533;

int readfile(char** s, const char* filename);
int compileAndAttachShaders(const char *vs, const char *fs, GLuint *program);

void mat4x4_gen_scale(mat4x4 a, float x, float y, float z);
void mat4x4_gen_translate(mat4x4 a, float x, float y, float z);
void mat4x4_gen_rotate(mat4x4 a, short y, short x, short z, float angle);
void mat4x4_gen_orthographic_projection(mat4x4 a, int WIDTH, int HEIGHT, float zFar, float zNear);
void mat4x4_debug_print(mat4x4 a);

#endif // UTILS_H