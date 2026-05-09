#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <string.h>
#include "../include/font.h"

font *font_load(unsigned int width, unsigned int height, const char *path)
{
	SDL_Surface *tex = IMG_Load(path);

	if (tex == NULL)
		return NULL;

	/* Coerce to a known pixel layout so the GL upload format matches the
	 * data regardless of what IMG_Load returned (RGB, BGR, RGBA, ...). */
	SDL_Surface *rgba = SDL_ConvertSurfaceFormat(tex, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(tex);

	if (rgba == NULL)
		return NULL;

	font *ret = malloc(sizeof(font));
	ret->char_width = width;
	ret->char_height = height;
	ret->w = rgba->w;
	ret->h = rgba->h;
	ret->path = malloc(strlen(path) + 1);
	strcpy(ret->path, path);

	glGenTextures(1, &(ret->textureID));
	glBindTexture(GL_TEXTURE_2D, ret->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba->w, rgba->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba->pixels);

	SDL_FreeSurface(rgba);

	return ret;
}
