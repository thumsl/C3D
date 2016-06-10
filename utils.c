//#include "utils.h"
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
    memset (*s, 0, st.st_size+1);

    FILE *f;
    f = fopen(filename,"rb");
    fread(*s, 1, st.st_size, f);

    return 1;
}

int compileAndAttachShaders(const char *vs, const char *fs, GLuint *program) {
    char *vertexSource, *fragmentSource;

    readfile(&vertexSource, vs);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char * const*)&vertexSource, NULL);
    glCompileShader(vertexShader);

    readfile(&fragmentSource, fs);
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
