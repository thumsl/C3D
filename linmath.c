#include "linmath.h"
#include <stdio.h>
#include <math.h>
#define toRadian(i) i*0.0174533f;

static void setMat4Identity(mat4 a) {
	a[0][0] = 1;	a[0][1] = 0;	a[0][2] = 0;	a[0][3] = 0;
	a[1][0] = 0;	a[1][1] = 1;	a[1][2] = 0;	a[1][3] = 0;
	a[2][0] = 0;	a[2][1] = 0;	a[2][2] = 1;	a[2][3] = 0;
	a[3][0] = 0;	a[3][1] = 0;	a[3][2] = 0;	a[3][3] = 1;
}

void mat4_copy(mat4 a, mat4 b) {
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			a[i][j] = b[i][j]
;}

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
	a[0][0] = x;	a[0][1] = 0;	a[0][2] = 0;	a[0][3] = 0;
	a[1][0] = 0;	a[1][1] = y;	a[1][2] = 0;	a[1][3] = 0;
	a[2][0] = 0;	a[2][1] = 0;	a[2][2] = z;	a[2][3] = 0;
	a[3][0] = 0;	a[3][1] = 0;	a[3][2] = 0;	a[3][3] = 1;
}

void mat4_gen_translate(mat4 a, float x, float y, float z) {
	a[0][0] = 1;	a[0][1] = 0;	a[0][2] = 0;	a[0][3] = x;
	a[1][0] = 0;	a[1][1] = 1;	a[1][2] = 0;	a[1][3] = y;
	a[2][0] = 0;	a[2][1] = 0;	a[2][2] = 1;	a[2][3] = z;
	a[3][0] = 0;	a[3][1] = 0;	a[3][2] = 0;	a[3][3] = 1;
}

void mat4_gen_rotate(mat4 a, short y, short x, short z, float angle) {
	angle = toRadian(angle);
	if (x) {
		a[0][0] = 1;			a[0][1] = 0;			a[0][2] = 0;			a[0][3] = 0;
		a[1][0] = 0;			a[1][1] = cos(angle);	a[1][2] = -sin(angle);	a[1][3] = 0;
		a[2][0] = 0;			a[2][1] = sin(angle);	a[2][2] =  cos(angle);	a[2][3] = 0;
		a[3][0] = 0;			a[3][1] = 0;			a[3][2] = 0;			a[3][3] = 1;
	}
	else if(y) {
		a[0][0] = cos(angle);	a[0][1] = 0;			a[0][2] = sin(angle);	a[0][3] = 0;
		a[1][0] = 0;			a[1][1] = 1;			a[1][2] = 0;			a[1][3] = 0;
		a[2][0] = -sin(angle);	a[2][1] = 0;			a[2][2] = cos(angle);	a[2][3] = 0;
		a[3][0] = 0;			a[3][1] = 0;			a[3][2] = 0;			a[3][3] = 1;
	}
	else if(z) {
		a[0][0] = cos(angle);	a[0][1] = -sin(angle);	a[0][2] = 0;	a[0][3] = 0;
		a[1][0] = sin(angle);	a[1][1] =  cos(angle);	a[1][2] = 0;	a[1][3] = 0;
		a[2][0] = 0;			a[2][1] = 0;			a[2][2] = 1;	a[2][3] = 0;
		a[3][0] = 0;			a[3][1] = 0;			a[3][2] = 0;	a[3][3] = 1;
	}
	else
		printf("[DEBUG] WARNING: mat4_gen_rotate() callled without defining axis of rotation.\n");
}

void mat4_gen_orthographic_projection(mat4 a, int WIDTH, int HEIGHT, float zFar, float zNear) {
		a[0][0] = 1.0f/WIDTH;	a[0][1] = 0;			a[0][2] = 0;					a[0][3] = 0;
		a[1][0] = 0;			a[1][1] =  1.0f/HEIGHT;	a[1][2] = 0;					a[1][3] = 0;
		a[2][0] = 0;			a[2][1] = 0;			a[2][2] = -2.0f/(zFar-zNear);	a[2][3] = -(zFar + zNear)/(zFar - zNear);
		a[3][0] = 0;			a[3][1] = 0;			a[3][2] = 0;					a[3][3] = 1;

		// TODO: perspective projection
}