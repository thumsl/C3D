#ifndef FONT_H
#define FONT_H

typedef struct font {
	unsigned int char_width;
	unsigned int char_height;
	unsigned int w;
	unsigned int h;
	unsigned int textureID;
	char *path;
} font;

font* font_load(unsigned int width, unsigned int height, const char *path);

#endif // FONT_H
