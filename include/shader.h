#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#define POSITION_LOCATION 0
#define TEXTURE_LOCATION 1
#define NORMAL_LOCATION 2

//	TODO: Use enum?
typedef struct {
	GLuint MVP;
	GLuint ambientLightColor;
	GLuint ambientLightIntensity;
	GLuint pointLightColor;
	GLuint pointLightPosition;
	GLuint attenuation;
} uniform_location;

typedef struct {
	GLuint program;
	uniform_location location;
} shader;

void initShader(shader* S);
int compileAndAttachShaders(shader *s, const char *vs, const char *fs);
#endif // SHADER_H