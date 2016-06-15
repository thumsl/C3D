#include "../include/utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

int readfile(char** s, const char* filename) {
    struct stat st;
    int i;

    if(stat(filename, &st) == -1)
        return 0;

    *s = malloc(st.st_size+1);
    memset (*s, 0, st.st_size+1); // \0

    FILE *f;
    f = fopen(filename,"rb");
    fread(*s, 1, st.st_size, f);

    return 1;
}

int compileAndAttachShaders(const char *vs, const char *fs, GLuint *program) {
    char *vertexSource, *fragmentSource;

    if (!readfile(&vertexSource, vs)) {
        fprintf(stderr, "Failed to open file %s\n", vs);
        return 0;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char * const*)&vertexSource, NULL);
    glCompileShader(vertexShader);

    if (!readfile(&fragmentSource, fs)) {
        fprintf(stderr, "Failed to open file %s\n", fs);
        return 0;
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char * const*)&fragmentSource, NULL);
    glCompileShader(fragmentShader);

    /** Check for compilation errors **/
    GLint status1, status2;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status1);
    if (status1 != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        fprintf(stderr, "%s", buffer);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status2);
    if (status2 != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        fprintf(stderr, "%s", buffer);
    }

    if (status1 != GL_TRUE || status2 != GL_TRUE)
        return 0;

    /** ---------------------------- **/

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    //TODO: create function to return uniform location
    *program = shaderProgram;

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

void mat4x4_gen_scale(mat4x4 a, float x, float y, float z) {
    a[0][0] = x;    a[0][1] = 0;    a[0][2] = 0;    a[0][3] = 0;
    a[1][0] = 0;    a[1][1] = y;    a[1][2] = 0;    a[1][3] = 0;
    a[2][0] = 0;    a[2][1] = 0;    a[2][2] = z;    a[2][3] = 0;
    a[3][0] = 0;    a[3][1] = 0;    a[3][2] = 0;    a[3][3] = 1;
}

void mat4x4_gen_translate(mat4x4 a, float x, float y, float z) {
    a[0][0] = 1;    a[0][1] = 0;    a[0][2] = 0;    a[0][3] = x;
    a[1][0] = 0;    a[1][1] = 1;    a[1][2] = 0;    a[1][3] = y;
    a[2][0] = 0;    a[2][1] = 0;    a[2][2] = 1;    a[2][3] = z;
    a[3][0] = 0;    a[3][1] = 0;    a[3][2] = 0;    a[3][3] = 1;
}

void mat4x4_gen_rotate(mat4x4 a, short y, short x, short z, float angle) {
    angle = toRadian(angle);
    if (x) {
        a[0][0] = 1;            a[0][1] = 0;            a[0][2] = 0;            a[0][3] = 0;
        a[1][0] = 0;            a[1][1] = cos(angle);   a[1][2] = -sin(angle);  a[1][3] = 0;
        a[2][0] = 0;            a[2][1] = sin(angle);   a[2][2] =  cos(angle);  a[2][3] = 0;
        a[3][0] = 0;            a[3][1] = 0;            a[3][2] = 0;            a[3][3] = 1;
    }
    else if(y) {
        a[0][0] = cos(angle);   a[0][1] = 0;            a[0][2] = sin(angle);   a[0][3] = 0;
        a[1][0] = 0;            a[1][1] = 1;            a[1][2] = 0;            a[1][3] = 0;
        a[2][0] = -sin(angle);  a[2][1] = 0;            a[2][2] = cos(angle);   a[2][3] = 0;
        a[3][0] = 0;            a[3][1] = 0;            a[3][2] = 0;            a[3][3] = 1;
    }
    else if(z) {
        a[0][0] = cos(angle);   a[0][1] = -sin(angle);  a[0][2] = 0;    a[0][3] = 0;
        a[1][0] = sin(angle);   a[1][1] =  cos(angle);  a[1][2] = 0;    a[1][3] = 0;
        a[2][0] = 0;            a[2][1] = 0;            a[2][2] = 1;    a[2][3] = 0;
        a[3][0] = 0;            a[3][1] = 0;            a[3][2] = 0;    a[3][3] = 1;
    }
    else
        printf("[DEBUG] WARNING: mat4x4_gen_rotate() callled without defining axis of rotation.\n");
    // TODO: allow rotating on 3 axis calling the function once
}

void mat4x4_gen_orthographic_projection(mat4x4 a, int WIDTH, int HEIGHT, float zFar, float zNear) {

        a[0][0] = 1.0f/WIDTH;   a[0][1] = 0;            a[0][2] = 0;                    a[0][3] = 0;
        a[1][0] = 0;            a[1][1] =  1.0f/HEIGHT; a[1][2] = 0;                    a[1][3] = 0;
        a[2][0] = 0;            a[2][1] = 0;            a[2][2] = -2.0f/(zFar-zNear);   a[2][3] = -(zFar + zNear)/(zFar - zNear);
        a[3][0] = 0;            a[3][1] = 0;            a[3][2] = 0;                    a[3][3] = 1;
        // TODO: perspective projection
}