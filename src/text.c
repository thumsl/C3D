#include <GL/glew.h>
#include <stdlib.h>
#include "../include/c3d.h"
#include "../include/font.h"
#include "../include/mesh.h"
#include "../include/text.h"

text *text_create(const char *string, font *F, float size, float x, float y)
{
	text *ret = malloc(sizeof(text));
	ret->modelList = list_create();
	ret->x = x;
	ret->y = y;
	ret->fontStyle = F;

	int i, row, col;
	float texture_width = (float)ret->fontStyle->char_width / ret->fontStyle->w, texture_height = (float)ret->fontStyle->char_height / ret->fontStyle->h;
	size /= 100.f;
	float font_size_x = size * (float)(ret->fontStyle->char_height / ret->fontStyle->char_width), font_size_y = size;

	GLuint indices[] = { 0, 3, 1, 3, 2, 1 };

	for (i = 0; string[i] != 0; i++, x += font_size_x) {
		mesh *model = malloc(sizeof(mesh));
		mesh_init(model);

		int ch = (unsigned char)string[i];
		row = (ch - 32) / (ret->fontStyle->w / ret->fontStyle->char_width);
		col = (ch - 32) % (ret->fontStyle->w / ret->fontStyle->char_width);

		model->textureOffsetX = texture_width * col;
		model->textureOffsetY = texture_height * row;

		GLfloat vertices[] = { x,
				       y,
				       0,
				       0,
				       texture_height,
				       0,
				       0,
				       0,
				       x,
				       y + font_size_y,
				       0,
				       0,
				       0,
				       0,
				       0,
				       0,
				       x + font_size_x,
				       y + font_size_y,
				       0,
				       texture_width,
				       0,
				       0,
				       0,
				       0,
				       x + font_size_x,
				       y,
				       0,
				       texture_width,
				       texture_height,
				       0,
				       0,
				       0 };

		model->indexCount = 6;
		model->vertexCount = 4;
		mesh_loadToVAO(model, vertices, indices);
		/* mesh_init allocated a fresh texture handle we never use; free
		 * it before reusing the font atlas to avoid leaking one texture
		 * per glyph per text_create call. */
		glDeleteTextures(1, &model->textureID);
		model->textureID = F->textureID;
		list_insert(ret->modelList, model);
	}

	ret->length = i;

	return ret;
}

void text_draw(text *T, shader *S, mat4x4 projection)
{
	shader_use(S);

	/* Text quads sit at z=0; under the engine's ortho they map to NDC
	 * z=-1 (depth 0, the closest possible value). With GL_DEPTH_TEST
	 * enabled globally, leaving depth-write on means text drawn before
	 * the 3D scene would occlude every later fragment. Disable depth
	 * writes for the duration of the text pass and restore the prior
	 * state afterwards. */
	GLboolean prev_depth_mask;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &prev_depth_mask);
	glDepthMask(GL_FALSE);

	node *aux = T->modelList->head;

	while (aux != NULL) {
		glUniform2f(S->uniforms[TEXT_SHADER_OFFSET], ((mesh *)aux->data)->textureOffsetX, ((mesh *)aux->data)->textureOffsetY);
		glUniformMatrix4fv(S->uniforms[TEXT_SHADER_PROJECTION], 1, 0, (GLfloat *)projection);

		mesh_draw((mesh *)aux->data, S, NULL, NULL);

		aux = aux->next;
	}

	glDepthMask(prev_depth_mask);
}

void text_update(text *T, const char *string)
{
	int i, row, col;
	node *aux = T->modelList->head;

	float texture_width = (float)T->fontStyle->char_width / T->fontStyle->w, texture_height = (float)T->fontStyle->char_height / T->fontStyle->h;

	for (i = 0; i < T->length; i++) {
		int ch = (unsigned char)string[i];
		row = (ch - 32) / (T->fontStyle->w / T->fontStyle->char_width);
		col = (ch - 32) % (T->fontStyle->w / T->fontStyle->char_width);

		((mesh *)aux->data)->textureOffsetX = texture_width * col;
		((mesh *)aux->data)->textureOffsetY = texture_height * row;

		aux = aux->next;
	}
}
