#ifndef LIGHT_H
#define LIGHT_H

#include "linmath.h"
#include "phongShader.h"
#include <GL/glew.h>

typedef struct ambientLight_s {
	vec3 color;
	float intensity;
} ambientLight;

typedef struct pointLight_s {
	vec3 color;
	vec3 position;
	float attenuation;
	float intensity;
} pointLight;

ambientLight* initAmbientLight(vec3 color, float intensity);
void setAmbientLight(ambientLight* ambient, phongShader *S);

pointLight* initPointLight(vec3 color, vec3 pos, float att, float intensity);
void setPointLight(pointLight* point, phongShader *S);

#endif // LIGHT_H