#include <stdio.h>
#include <stdlib.h>
#include "../include/shader.h"
#include "../include/utils.h"

void shader_getLocations(shader* S) {
	S->location.MVP = glGetUniformLocation(S->program, "MVP");
    S->location.ModelView = glGetUniformLocation(S->program, "ModelView");
    S->location.Projection = glGetUniformLocation(S->program, "Projection");
    S->location.Transform = glGetUniformLocation(S->program, "Transform");
	S->location.ambientLightColor = glGetUniformLocation(S->program, "ambient.color"); // TODO: check for errors
    S->location.ambientLightIntensity = glGetUniformLocation(S->program, "ambient.intensity");
    S->location.pointLightPosition = glGetUniformLocation(S->program, "point.position");
    S->location.pointLightColor = glGetUniformLocation(S->program, "point.color");
    S->location.pointLightAttenuation = glGetUniformLocation(S->program, "point.attenuation");
    S->location.pointLightIntensity = glGetUniformLocation(S->program, "point.intensity");
    S->location.eyePos = glGetUniformLocation(S->program, "eyePos");
    S->location.specularPower = glGetUniformLocation(S->program, "specularPower");
    S->location.specularIntensity = glGetUniformLocation(S->program, "specularIntensity");

    S->location.skyColor = glGetUniformLocation(S->program, "skyColor");
    S->location.fogDensity = glGetUniformLocation(S->program, "fogDensity");
    S->location.fogGradient = glGetUniformLocation(S->program, "fogGradient");
}

shader* shader_loadFromFile(const char *vs, const char *fs) {
    char *vertexSource, *fragmentSource;

    if (!readfile(&vertexSource, vs)) {
        fprintf(stderr, "Failed to open file %s\n", vs);
        return NULL;
    }

    if (!readfile(&fragmentSource, fs)) {
        fprintf(stderr, "Failed to open file %s\n", fs);
        return NULL;
    }

    shader *S = (shader*)malloc(sizeof(shader));

    S->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(S->vertexShader, 1, (const char * const*)&vertexSource, NULL);
    glCompileShader(S->vertexShader);

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
        return NULL;

    /** ---------------------------- **/

    S->program = glCreateProgram();

    glAttachShader(S->program, S->vertexShader);
    glAttachShader(S->program, S->fragmentShader);
    glLinkProgram(S->program);

    vec3 color = {0.0f, 0.0f, 0.0f};
    shader_use(S);
    shader_getLocations(S);
    shader_setFogParams(S, 0, 1);
    shader_setSkyColor(S, color);

    return S;
}

void shader_use(shader* S) {
    glUseProgram(S->program);
}

void shader_setSkyColor(shader *S, vec3 color) {
    glUniform3fv(S->location.skyColor, 1, (GLfloat*)color);
}

void shader_setFogParams(shader *S, float density, float gradient) {
    glUniform1f(S->location.fogDensity, density);
    glUniform1f(S->location.fogGradient, gradient);
}