#ifndef LIGHT_H
#define LIGHT_H

#include "linmath.h"
#include "shader.h"
#include <GL/glew.h>

typedef struct {
	GLuint directionLocation;
	GLuint colorLocation;
	vec3 lightDirection;
	vec3 lightColor;
} diffuseLight;

void initDiffuseLight(diffuseLight* diffuse, vec3 direction, vec3 color); // inline?s
void setDiffuseLight(diffuseLight* diffuse, shader* S);
#endif // LIGHT_H