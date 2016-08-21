#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <string.h>
#include "../include/font.h"

font* font_load(unsigned int width, unsigned int height, const char *path) {
	SDL_Surface *tex = IMG_Load(path);

	if (tex == NULL)
		return NULL;

	font *ret = (font*)malloc(sizeof(font));
	ret->char_width = width;
	ret->char_height = height;
	ret->w = tex->w;
	ret->h = tex->h;
	ret->path = (char*)malloc(strlen(path) * sizeof(char));
	strcpy(ret->path, path);

	glGenTextures(1, &(ret->textureID));
	glBindTexture(GL_TEXTURE_2D, ret->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);

	SDL_FreeSurface(tex);

	return ret;
}