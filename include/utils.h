#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include "linmath.h"
#include <time.h>

#define access_2df_array(a, size, i, j) ((*((float*)a + (i)*size + (j))))
#define setRandSeed() srand(time(NULL))
#define randomValue(i) (rand() / (RAND_MAX / i + 1))
#define toRadian(i) (i*0.0174533f)

int readfile(char** s, const char* filename);

void vec4_copy(vec4 r, vec4 a);
void vec3_copy(vec3 r, vec3 a);
void mat4x4_debug_print(mat4x4 a);

#endif // UTILS_H