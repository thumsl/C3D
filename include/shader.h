#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#define POSITION_LOCATION 0
#define TEXTURE_LOCATION 1
#define NORMAL_LOCATION 2

//	TODO: Use enum?
typedef struct {
	GLuint MVP;
	GLuint Transform;
	GLuint ambientLightColor;
	GLuint ambientLightIntensity;
	GLuint pointLightColor;
	GLuint pointLightPosition;
	GLuint pointLightAttenuation;
	GLuint pointLightIntensity;
	GLuint eyePos;
	GLuint specularPower;
	GLuint specularIntensity;
} uniform_location;

typedef struct {
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	uniform_location location;
} shader;

void shader_getLocations(shader* S);
int shader_loadFromFile(shader* S, const char *vs, const char *fs);
void shader_use(shader* S);
#endif // SHADER_H