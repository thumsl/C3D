#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>

static void setMat4Identity(mat4 a) {
	a[0][0] = 1;	a[0][1] = 0;	a[0][2] = 0;	a[0][3] = 0;
	a[1][0] = 0;	a[1][1] = 1;	a[1][2] = 0;	a[1][3] = 0;
	a[2][0] = 0;	a[2][1] = 0;	a[2][2] = 1;	a[2][3] = 0;
	a[3][0] = 0;	a[3][1] = 0;	a[3][2] = 0;	a[3][3] = 1;
}

void mat4_mult_constant(mat4 a, float c) {
	a[0][0] *= c;	a[0][1] *= c;	a[0][2] *= c;	a[0][3] *= 1;
	a[1][0] *= c;	a[1][1] *= c;	a[1][2] *= c;	a[1][3] *= 1;
	a[2][0] *= c;	a[2][1] *= c;	a[2][2] *= c;	a[2][3] *= 1;
	a[3][0] *= c;	a[3][1] *= c;	a[3][2] *= c;	a[3][3] *= 1;
}

void mat4_debug_print(mat4 a) {
	int i, j;
	printf("------------------------------------\n");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			printf("%f ", a[i][j]);
		putchar('\n');
	}
	printf("------------------------------------\n");
}

void mat4_mult(mat4 a, mat4 b, mat4 r) {
	int i, j, k;
	float sum;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			r[i][j] = 0;
			for (k = 0; k < 4; k++)
				r[i][j] += a[k][j] * b[i][k];
		}
}

void mat4_gen_scale(mat4 a, float x, float y, float z) {
	a[0][0] = x;	a[0][1] = 0;	a[0][2] = 0;	a[0][3] = 1;
	a[1][0] = 0;	a[1][1] = y;	a[1][2] = 0;	a[1][3] = 1;
	a[2][0] = 0;	a[2][1] = 0;	a[2][2] = z;	a[2][3] = 1;
	a[3][0] = 0;	a[3][1] = 0;	a[3][2] = 0;	a[3][3] = 1;
}

//mat4 mat4_mult_vec4(mat4 a, float x, float y, float z, float w) {}

