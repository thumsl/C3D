#include <GL/glew.h>
#include "../include/textShader.h"

void textShader_getUniformLocations(shader *S) {
	char *uniformNames[] = {
		"projection",
		"offset"
	};

	int i;
	
	for (i = 0; i < TEXT_SHADER_UNIFORM_COUNT; i++)
		S->uniforms[i] = glGetUniformLocation(S->program, uniformNames[i]);
}
