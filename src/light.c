#include "../include/light.h"
#include "../include/utils.h"

void initDiffuseLight(diffuseLight* diffuse, vec3 direction, vec3 color) {
	vec3_copy(diffuse->lightDirection, direction);
	vec3_copy(diffuse->lightColor, color);
}

void setDiffuseLight(diffuseLight* diffuse, shader* S) {
	glUniform3fv(S->diffuseLightDirectionLocation, 1, (GLfloat*)diffuse->lightDirection);
	glUniform3fv(S->diffuseLightColorLocation, 1, (GLfloat*)diffuse->lightColor);
}