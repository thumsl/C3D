#include <stdio.h>
#include <stdlib.h>
#include "../include/shader.h"
#include "../include/utils.h"

bool shader_loadFromFile(const char *vs, const char *fs, GLuint *program) {
    char *vertexSource, *fragmentSource;

    if (!readfile(&vertexSource, vs)) {
        fprintf(stderr, "Failed to open file %s\n", vs);
        return false;
    }

    if (!readfile(&fragmentSource, fs)) {
        fprintf(stderr, "Failed to open file %s\n", fs);
        return false;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char * const*)&vertexSource, NULL);
    glCompileShader(vertexShader);

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
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        fprintf(stderr, "%s", buffer);
    }

    if (status1 != GL_TRUE || status2 != GL_TRUE)
        return NULL;

    /** ---------------------------- **/

    *program = glCreateProgram();

    glAttachShader(*program, vertexShader);
    glAttachShader(*program, fragmentShader);
    glLinkProgram(*program);

    return true;
}

void shader_use(GLuint program) {
    glUseProgram(program);
}