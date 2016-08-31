/*!	
	@file shader.h
	@author Thums
	@brief All things shader
*/

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdbool.h>
#include "linmath.h"

/*!
    This function will load a shader from the disk, compile, attach and then link it.

    @param vs Path to the vertex shader file
    @param fs Path to the fragment shader file
    @return Pointer to shader if successful, otherwise NULL
*/

typedef enum {
	PHONG,
	TEXT
} shaderType;

typedef struct shader {
	GLuint program;
	shaderType type;
	GLuint *uniforms;
} shader;

static void shader_getUniformLocations(shader *S);

shader* shader_loadFromFile(const char *vs, const char *fs, shaderType type);
void shader_use(shader *S);

#endif // SHADER_H
