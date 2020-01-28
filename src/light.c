#include <stdlib.h>
#include "../include/light.h"
#include "../include/utils.h"

// TODO: move this to a new file called phongShader
ambientLight* initAmbientLight(vec3 color, float intensity) {
	ambientLight *ambient = malloc(sizeof(ambientLight));
	if (ambient == NULL)
		return NULL;

	vec3_copy(ambient->color, color);
	ambient->intensity = intensity;

	return ambient;
}

void setAmbientLight(ambientLight *ambient, shader *S) {
	glUniform3fv(S->uniforms[AMBIENT_LIGHT_COLOR], 1, (GLfloat*)ambient->color);
	glUniform1f(S->uniforms[AMBIENT_LIGHT_INTENSITY], ambient->intensity);
}

pointLight* initPointLight(vec3 color, vec3 pos, float att, float intensity) {
	pointLight *point = malloc(sizeof(pointLight));
	if (point == NULL)
		return NULL;

	vec3_copy(point->color, color);
	vec3_copy(point->position, pos);
	point->attenuation = att;
	point->intensity = intensity;

	return point;
}

void setPointLight(pointLight *point, shader *S) {
	glUniform3fv(S->uniforms[POINT_LIGHT_POSITION], 1, (GLfloat*)point->position);
	glUniform3fv(S->uniforms[POINT_LIGHT_COLOR], 1, (GLfloat*)point->color);
	glUniform1f(S->uniforms[POINT_LIGHT_ATTENUATION], point->attenuation);
	glUniform1f(S->uniforms[POINT_LIGHT_INTENSITY], point->intensity);
}
