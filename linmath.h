#ifndef LINMATH_H
#define LINMATH_H

typedef float mat4[4][4];

static void setMat4Identity();

void mat4_copy(mat4 a, mat4 b);
void mat4_mult_constant(mat4 a, float c);
void mat4_mult(mat4 a, mat4 b, mat4 r);
void mat4_mult_vec4(mat4 a, float x, float y, float z, float w);

void mat4_gen_scale(mat4 a, float x, float y, float z);
void mat4_gen_translate(mat4 a, float x, float y, float z);
void mat4_gen_rotate(mat4 a, short y, short x, short z, float angle);
void mat4_gen_orthographic_projection(mat4 a, int WIDTH, int HEIGHT, float zFar, float zNear);

void mat4_debug_print(mat4 a);

#endif
