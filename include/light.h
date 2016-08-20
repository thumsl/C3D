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
	float intensity;
} pointLight;

ambientLight* initAmbientLight(vec3 color, float intensity);
void setAmbientLight(ambientLight* ambient, shader *S);

pointLight* initPointLight(vec3 color, vec3 pos, float att, float intensity);
void setPointLight(pointLight* point, shader *S);

#endif // LIGHT_H