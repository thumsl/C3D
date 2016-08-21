#include <stdlib.h>
#include "../include/light.h"
#include "../include/utils.h"

// TODO: move this to a new file called phongShader
ambientLight* initAmbientLight(vec3 color, float intensity) {
	ambientLight *ambient = (ambientLight*)malloc(sizeof(ambientLight));
	if (ambient == NULL)
		return NULL;

	vec3_copy(ambient->color, color);
	ambient->intensity = intensity;

	return ambient;
}

void setAmbientLight(ambientLight *ambient, phongShader *S) {
	glUniform3fv(S->locations.ambientLightColor, 1, (GLfloat*)ambient->color);
	glUniform1f(S->locations.ambientLightIntensity, ambient->intensity);
}

pointLight* initPointLight(vec3 color, vec3 pos, float att, float intensity) {
	pointLight *point = (pointLight*)malloc(sizeof(pointLight));
	if (point == NULL)
		return NULL;

	vec3_copy(point->color, color);
	vec3_copy(point->position, pos);
	point->attenuation = att;
	point->intensity = intensity;

	return point;
}

void setPointLight(pointLight *point, phongShader *S) {
	glUniform3fv(S->locations.pointLightPosition, 1, (GLfloat*)point->position);
	glUniform3fv(S->locations.pointLightColor, 1, (GLfloat*)point->color);
	glUniform1f(S->locations.pointLightAttenuation, point->attenuation);
	glUniform1f(S->locations.pointLightIntensity, point->intensity);
}