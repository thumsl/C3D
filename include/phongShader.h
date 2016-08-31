#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include <GL/glew.h>
#include <stdlib.h>
#include "camera.h"
#include "linmath.h"
#include "mesh.h"
#include "shader.h"

#define POSITION_LOCATION 0
#define TEXTURE_LOCATION 1
#define NORMAL_LOCATION 2

typedef struct camera camera;
typedef struct mesh mesh;

typedef enum {
	MVP = 0,
	MODEL_VIEW,
	PROJECTION,
	TRANSFORM,
	AMBIENT_LIGHT_COLOR,
	AMBIENT_LIGHT_INTENSITY,
	POINT_LIGHT_COLOR,
	POINT_LIGHT_POSITION,
	POINT_LIGHT_INTENSITY,
	POINT_LIGHT_ATTENUATION,
	EYE_POSITION,
	SPECULAR_POWER,
	SPECULAR_INTENSITY,
	SKY_COLOR,
	FOG_DENSITY,
	FOG_GRADIENT,
	PHONG_SHADER_UNIFORM_COUNT
} phongShaderUniforms;

void phongShader_getUniformLocations(shader *S);
void phongShader_updateUniforms(mesh *model, shader *S, camera *C, mat4x4 projection);
void phongShader_init(shader *S);

void phongShader_setSkyColor(shader* S, vec3 color);
void phongShader_setFogParams(shader *S, float density, float gradient);

#endif // PHONG_SHADER_H
