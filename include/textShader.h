#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

#include "shader.h"

typedef enum {
	TEXT_SHADER_PROJECTION = 0,
	TEXT_SHADER_OFFSET,
	TEXT_SHADER_UNIFORM_COUNT
} textShaderUniforms; 

void textShader_getUniformLocations(shader *S);

#endif // TEXT_SHADER_H
