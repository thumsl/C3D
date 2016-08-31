#include <stdlib.h>
#include <stdio.h>
#include "../include/phongShader.h"
#include "../include/shader.h"

void phongShader_getUniformLocations(shader *S) {
	char *uniformNames[] = {
		"MVP",
		"ModelView",
		"Projection",
		"Transform",
		"ambient.color",
		"ambient.intensity",
		"point.color",
		"point.position",
		"point.intensity",
		"point.attenuation",
		"eyePos",
		"specularPower",
		"specularIntensity",
		"skyColor",
		"fogDensity",
		"fogGradient"
	};

	int i;

	for (i = 0; i < PHONG_SHADER_UNIFORM_COUNT; i++)
		S->uniforms[i] = glGetUniformLocation(S->program, uniformNames[i]);

	for (i = 0; i < PHONG_SHADER_UNIFORM_COUNT; i++) {
		printf("S->uniforms[%d] = %d\n", i, S->uniforms[i]);
	}
}


void phongShader_setSkyColor(shader *S, vec3 color) {
	glUniform3fv(S->uniforms[SKY_COLOR], 1, (GLfloat*)color);
}

void phongShader_setFogParams(shader *S, float density, float gradient) {
	glUniform1f(S->uniforms[FOG_DENSITY], density);
	glUniform1f(S->uniforms[FOG_GRADIENT], gradient);
}

void phongShader_updateUniforms(mesh *model, shader *S, camera *C, mat4x4 projection) {
	mat4x4 modelView;
	mat4x4_mul(modelView, C->view, model->matrices.transform);

	glUniformMatrix4fv(S->uniforms[MODEL_VIEW], 1, 0, (GLfloat*)modelView);
	glUniformMatrix4fv(S->uniforms[PROJECTION], 1, 0, (GLfloat*)projection);
	glUniformMatrix4fv(S->uniforms[TRANSFORM], 1, 0, (GLfloat*)model->matrices.transform);
	glUniform3fv(S->uniforms[EYE_POSITION], 1, (GLfloat*)C->eye);
	glUniform1f(S->uniforms[SPECULAR_POWER], model->mat.specularPower);
	glUniform1f(S->uniforms[SPECULAR_INTENSITY], model->mat.specularIntensity);
}

void phongShader_init(shader *S) {
	vec3 color = {1.0f, 0.0f, 0.0f};
	
	phongShader_setSkyColor(S, color);
	phongShader_setFogParams(S, 0.5f, 1.0f);

	color[0] = color[1] = color[2] = 1.0f;
	glUniform3fv(S->uniforms[AMBIENT_LIGHT_COLOR], 1, (GLfloat*)color);
	glUniform1f(S->uniforms[AMBIENT_LIGHT_INTENSITY], 1.0f);
}


