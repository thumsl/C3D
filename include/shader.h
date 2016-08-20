/*!	
	@file shader.h
	@author Thums
	@brief All things shader
*/

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include "linmath.h"

#define POSITION_LOCATION 0
#define TEXTURE_LOCATION 1
#define NORMAL_LOCATION 2

//	TODO: Use enum?
typedef struct {
	GLuint MVP;
	GLuint ModelView;
	GLuint Projection;
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

	GLuint skyColor;
	GLuint fogDensity;
	GLuint fogGradient;
} uniform_location;

typedef struct {
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	uniform_location location;
} shader;

void shader_getLocations(shader* S);

/*!
    This function will load a shader from the disk, compile, attach and then link it.

    It will also set the default values for sky color (black), fog density (0), fog
    gradient (1) and apply the shader.

    @param vs Path to the vertex shader file
    @param fs Path to the fragment shader file
    @return Pointer to shader if successful, otherwise NULL
*/
shader* shader_loadFromFile(const char *vs, const char *fs);
void shader_use(shader* S);
void shader_setSkyColor(shader* S, vec3 color);
void shader_setFogParams(shader *S, float density, float gradient);
#endif // SHADER_H