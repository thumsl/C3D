#include <stdlib.h>
#include "../include/textShader.h"
#include "../include/shader.h"

textShader* textShader_load(const char *vs, const char *fs) {
    textShader *ret = (textShader*)malloc(sizeof(textShader));

    if (!shader_loadFromFile(vs, fs, &(ret->program))) {
    	free (ret);
    	return NULL;
    }

    shader_use(ret->program);
    textShader_getLocations(ret);

    return ret;
}

static void textShader_getLocations(textShader *S) {
	S->locations.projection = glGetUniformLocation(S->program, "projection");
	S->locations.offset = glGetUniformLocation(S->program, "offset");
}