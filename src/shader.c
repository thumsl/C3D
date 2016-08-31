#include <stdio.h>
#include <stdlib.h>
#include "../include/shader.h"
#include "../include/textShader.h"
#include "../include/phongShader.h"
#include "../include/utils.h"

static void shader_getUniformLocations(shader *S) {
	int i;

	S->uniforms = (GLuint*) malloc(sizeof(GLuint) * PHONG_SHADER_UNIFORM_COUNT * 2);

	switch (S->type) {
		case PHONG:
			phongShader_getUniformLocations(S);
			phongShader_init(S);
			break;
		case TEXT:
			textShader_getUniformLocations(S);
			break;
	}
}

shader* shader_loadFromFile(const char *vs, const char *fs, shaderType type) {
    char *vertexSource, *fragmentSource;

    if (!readfile(&vertexSource, vs)) {
        fprintf(stderr, "Failed to open file %s\n", vs);
        return NULL;
    }

    if (!readfile(&fragmentSource, fs)) {
        fprintf(stderr, "Failed to open file %s\n", fs);
        return NULL;
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

    shader *S = (shader*) malloc(sizeof(shader));

    S->program = glCreateProgram();
    S->type = type;

    glAttachShader(S->program, vertexShader);
    glAttachShader(S->program, fragmentShader);
    glLinkProgram(S->program);

    shader_use(S);

    shader_getUniformLocations(S);

    return S;
}

void shader_use(shader *S) {
    glUseProgram(S->program);
}
