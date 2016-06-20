#include "../include/utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

void vec4_copy(vec4 r, vec4 a) {
    int i;
    for (i = 0; i < 4; r[i] = a[i++]);
}

void vec3_copy(vec3 r, vec3 a) {
    int i;
    for (i = 0; i < 3; r[i] = a[i++]);
}

int readfile(char** s, const char* filename) {
    struct stat st;
    int i;

    if(stat(filename, &st) == -1)
        return 0;

    *s = malloc(st.st_size+2);
    memset (*s, 0, st.st_size+2); // \0

    FILE *f;
    f = fopen(filename,"rb");
    fread(*s, 1, st.st_size, f);
    
    return 1;
}

void mat4x4_debug_print(mat4x4 a) {
    int i, j;
    printf("------------------------------------\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++)
            printf("%f ", a[i][j]);
        putchar('\n');
    }
    printf("------------------------------------\n");
}

void mat4x4_mult(mat4x4 a, mat4x4 b, mat4x4 r) {
    int i, j, k;
    float sum;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) {
            r[i][j] = 0;
            for (k = 0; k < 4; k++)
                r[i][j] += a[k][j] * b[i][k];
        }
}