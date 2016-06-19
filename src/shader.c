#include <stdio.h>
#include "../include/shader.h"
#include "../include/utils.h"

void initShader(shader* S) {
	S->MVPLocation = glGetUniformLocation(S->program, "MVP");
	S->diffuseLightDirectionLocation = glGetUniformLocation(S->program, "diffuseLightDirection");
	S->diffuseLightColorLocation = glGetUniformLocation(S->program, "diffuseLightColor"); // TODO: check for errors
}

int compileAndAttachShaders(shader *S, const char *vs, const char *fs) {
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

    S->program = glCreateProgram();

    glAttachShader(S->program, vertexShader);
    glAttachShader(S->program, fragmentShader);
    glLinkProgram(S->program);
    glUseProgram(S->program);

    return 1;
}