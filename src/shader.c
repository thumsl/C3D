#include <stdio.h>
#include "../include/shader.h"
#include "../include/utils.h"

void shader_getLocations(shader* S) {
	S->location.MVP = glGetUniformLocation(S->program, "MVP");
    S->location.Transform = glGetUniformLocation(S->program, "Transform");
	S->location.ambientLightColor = glGetUniformLocation(S->program, "ambient.color"); // TODO: check for errors
    S->location.ambientLightIntensity = glGetUniformLocation(S->program, "ambient.intensity");
    S->location.pointLightPosition = glGetUniformLocation(S->program, "point.position");
    S->location.pointLightColor = glGetUniformLocation(S->program, "point.color");
    S->location.pointLightAttenuation = glGetUniformLocation(S->program, "point.attenuation");
    S->location.pointLightIntensity = glGetUniformLocation(S->program, "point.intensity");
    S->location.eyePos = glGetUniformLocation(S->program, "eyePos");
}

int shader_loadFromFile(shader *S, const char *vs, const char *fs) {
    char *vertexSource, *fragmentSource;

    if (!readfile(&vertexSource, vs)) {
        fprintf(stderr, "Failed to open file %s\n", vs);
        return 0;
    }

    S->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(S->vertexShader, 1, (const char * const*)&vertexSource, NULL);
    glCompileShader(S->vertexShader);

    if (!readfile(&fragmentSource, fs)) {
        fprintf(stderr, "Failed to open file %s\n", fs);
        return 0;
    }
    S->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(S->fragmentShader, 1, (const char * const*)&fragmentSource, NULL);
    glCompileShader(S->fragmentShader);

    /** Check for compilation errors **/
    GLint status1, status2;

    glGetShaderiv(S->vertexShader, GL_COMPILE_STATUS, &status1);
    if (status1 != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(S->vertexShader, 512, NULL, buffer);
        fprintf(stderr, "%s", buffer);
    }

    glGetShaderiv(S->fragmentShader, GL_COMPILE_STATUS, &status2);
    if (status2 != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(S->fragmentShader, 512, NULL, buffer);
        fprintf(stderr, "%s", buffer);
    }

    if (status1 != GL_TRUE || status2 != GL_TRUE)
        return 0;

    /** ---------------------------- **/

    S->program = glCreateProgram();

    glAttachShader(S->program, S->vertexShader);
    glAttachShader(S->program, S->fragmentShader);
    glLinkProgram(S->program);
    return 1;
}

void shader_use(shader* S) {
    glUseProgram(S->program);

}