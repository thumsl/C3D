#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

#include <GL/glew.h>

typedef struct textLocations_s {
	GLuint projection;
	GLuint offset;
} textLocations;

typedef struct textShader_h {
	GLuint program;
	textLocations locations;
} textShader;

textShader* textShader_load(const char *vs, const char *fs);
static void textShader_getLocations(textShader *S);
#endif // TEXT_SHADER_H