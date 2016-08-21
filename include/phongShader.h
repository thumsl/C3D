#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include <GL/glew.h>
#include <stdlib.h>
#include "camera.h"
#include "linmath.h"
#include "list.h"
#include "mesh.h"

#define POSITION_LOCATION 0
#define TEXTURE_LOCATION 1
#define NORMAL_LOCATION 2

typedef struct camera_s camera;
typedef struct mesh_s mesh;

//	TODO: Use enum?
typedef struct phongLocations_s {
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
} phongLocations;

typedef struct PhongShader_s {
	GLuint program;
	phongLocations locations;
} phongShader;

phongShader* phongShader_load(const char *vs, const char *fs);
static void phongShader_getLocations(phongShader* S);
void phongShader_setSkyColor(phongShader* S, vec3 color);
void phongShader_setFogParams(phongShader *S, float density, float gradient);
void phongShader_drawList(linkedList *modelList, phongShader *S, camera *C, mat4x4 projection);

#endif // PHONG_SHADER_H