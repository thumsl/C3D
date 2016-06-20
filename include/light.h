#ifndef LIGHT_H
#define LIGHT_H

#include "linmath.h"
#include "shader.h"
#include <GL/glew.h>

typedef struct {
	vec3 color;
	float intensity;
} ambientLight;

typedef struct {
	vec3 color;
	vec3 position;
	float attenuation;
} pointLight;

void initAmbientLight(ambientLight* ambient, vec3 color, float intensity);
void setAmbientLight(ambientLight* ambient, shader *S);

void initPointLight(pointLight* point, vec3 color, vec3 pos, float att);
void setPointLight(pointLight* point, shader *S);

#endif // LIGHT_H