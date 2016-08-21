#include <GL/glew.h>
#include <stdlib.h>
#include "../include/engine.h"
#include "../include/font.h"
#include "../include/mesh.h"
#include "../include/text.h"

text* text_create(const char *string, font *F, float size, float x, float y) {
	text *ret = (text*)malloc(sizeof(text));
	ret->modelList = list_create();
	ret->x = x;
	ret->y = y;
	ret->fontStyle = F;

	int i, row, col;
	float texture_width = (float)ret->fontStyle->char_width/ret->fontStyle->w, texture_height = (float)ret->fontStyle->char_height/ret->fontStyle->h;
	float font_size_x = (size/(WIDTH/10.f)) * (float)(ret->fontStyle->char_height/ret->fontStyle->char_width), font_size_y = size/(HEIGHT/10.f);

	GLuint indices[] = {
		0, 3, 1,
		3, 2, 1
	};

	for (i = 0; string[i] != 0; i++, x += font_size_x) {
		mesh *model = (mesh*)malloc(sizeof(mesh));
		mesh_init(model);

		row = (string[i]-32) / (ret->fontStyle->w / ret->fontStyle->char_width);
		col = (string[i]-32) % (ret->fontStyle->w / ret->fontStyle->char_width);

		model->textureOffsetX = texture_width * col;
		model->textureOffsetY = texture_height * row;

		GLfloat vertices[] = {
			x,					y,					0,	0,				texture_height,	0,	0,	0,
			x,					y + font_size_y,	0,	0,				0,				0,	0,	0,
			x + font_size_x,	y + font_size_y,	0,	texture_width,	0,				0,	0,	0,
			x + font_size_x, 	y,					0,	texture_width,	texture_height,	0,	0,	0
		};


		model->indexCount = 6;
		model->vertexCount = 4;
		mesh_loadToVAO(model, vertices, indices);
		model->textureID = F->textureID;
		list_insert(ret->modelList, model);
	}

	ret->length = i;

	return ret;
}


void text_draw(text *T, mat4x4 projection, textShader *S) {
	shader_use(S->program);
	glUniformMatrix4fv(S->locations.projection, 1, 0, (GLfloat*)projection);

	node *aux = T->modelList->head;
	while (aux != NULL) {
		glUniform2f(S->locations.offset, ((mesh*)aux->data)->textureOffsetX, ((mesh*)aux->data)->textureOffsetY);
		mesh_draw((mesh*)aux->data);
		aux = aux->next;
	}
}

void text_update(text *T, const char *string) {
	int i, row, col;
	node *aux = T->modelList->head;

	float texture_width = (float)T->fontStyle->char_width/T->fontStyle->w, texture_height = (float)T->fontStyle->char_height/T->fontStyle->h;

	for (i = 0; i < T->length; i++) {
		row = (string[i]-32) / (T->fontStyle->w / T->fontStyle->char_width);
		col = (string[i]-32) % (T->fontStyle->w / T->fontStyle->char_width);

		((mesh*)aux->data)->textureOffsetX = texture_width * col;
		((mesh*)aux->data)->textureOffsetY = texture_height * row;

		aux = aux->next;
	}
}