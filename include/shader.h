#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#define POSITION_LOCATION 0
#define TEXTURE_LOCATION 1
#define NORMAL_LOCATION 2

typedef struct {
	GLuint MVPLocation;
	GLuint diffuseLightDirectionLocation;
	GLuint diffuseLightColorLocation;
	GLuint program;
} shader;

void initShader(shader* S);
int compileAndAttachShaders(shader *s, const char *vs, const char *fs);
#endif // SHADER_H