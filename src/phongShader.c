#include <stdlib.h>
#include <stdio.h>
#include "../include/phongShader.h"
#include "../include/shader.h"

phongShader* phongShader_load(const char *vs, const char *fs) {
    phongShader *ret = (phongShader*)malloc(sizeof(phongShader));

    if (!shader_loadFromFile(vs, fs, &(ret->program))) {
        free(ret);
        return NULL;
    }

    shader_use(ret->program);
    vec3 color = {0.0f, 0.0f, 0.0f};
    phongShader_getLocations(ret);
    phongShader_setFogParams(ret, 0, 1);
    phongShader_setSkyColor(ret, color);

    return ret;
}

// TODO: Make static
static void phongShader_getLocations(phongShader* S) {
	S->locations.MVP = glGetUniformLocation(S->program, "MVP");
    S->locations.ModelView = glGetUniformLocation(S->program, "ModelView");
    S->locations.Projection = glGetUniformLocation(S->program, "Projection");
    S->locations.Transform = glGetUniformLocation(S->program, "Transform");
	S->locations.ambientLightColor = glGetUniformLocation(S->program, "ambient.color"); // TODO: check for errors
    S->locations.ambientLightIntensity = glGetUniformLocation(S->program, "ambient.intensity");
    S->locations.pointLightPosition = glGetUniformLocation(S->program, "point.position");
    S->locations.pointLightColor = glGetUniformLocation(S->program, "point.color");
    S->locations.pointLightAttenuation = glGetUniformLocation(S->program, "point.attenuation");
    S->locations.pointLightIntensity = glGetUniformLocation(S->program, "point.intensity");
    S->locations.eyePos = glGetUniformLocation(S->program, "eyePos");
    S->locations.specularPower = glGetUniformLocation(S->program, "specularPower");
    S->locations.specularIntensity = glGetUniformLocation(S->program, "specularIntensity");
    S->locations.skyColor = glGetUniformLocation(S->program, "skyColor");
    S->locations.fogDensity = glGetUniformLocation(S->program, "fogDensity");
    S->locations.fogGradient = glGetUniformLocation(S->program, "fogGradient");
}

void phongShader_setSkyColor(phongShader *S, vec3 color) {
    glUniform3fv(S->locations.skyColor, 1, (GLfloat*)color);
}

void phongShader_setFogParams(phongShader *S, float density, float gradient) {
    glUniform1f(S->locations.fogDensity, density);
    glUniform1f(S->locations.fogGradient, gradient);
}

void phongShader_drawList(linkedList *modelList, phongShader *S, camera *C, mat4x4 projection) {
    node *aux = modelList->head;

    while (aux != NULL) {
        mesh_updateModel((mesh*)aux->data);

        //mat4x4 model_view_projection;
        mat4x4 modelView;
        mat4x4_mul(modelView, C->view, ((mesh*)aux->data)->transform.model);
        //mat4x4_mul(model_view_projection, projection, model_view_projection);

        glUniformMatrix4fv(S->locations.ModelView, 1, 0, (GLfloat*)modelView);
        glUniformMatrix4fv(S->locations.Projection, 1, 0, (GLfloat*)projection);
        glUniformMatrix4fv(S->locations.Transform, 1, 0, (GLfloat*)((mesh*)aux->data)->transform.model);
        glUniform3fv(S->locations.eyePos, 1, (GLfloat*)C->eye);
        glUniform1f(S->locations.specularPower, ((mesh*)aux->data)->mat.specularPower);
        glUniform1f(S->locations.specularIntensity, ((mesh*)aux->data)->mat.specularIntensity);

        mesh_draw((mesh*)aux->data);
        aux = aux->next;
    }
}