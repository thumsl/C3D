/*!
	@file text.h
	@brief Allows text to be rendered to the screen.
	@author Luan Cristian Thums
*/

#ifndef TEXT_H
#define TEXT_H
#define TEXTURE_SIZE 512

#include "linmath.h"
#include "list.h"
#include "font.h"
#include "textShader.h"

/*!
	@brief This structure stores all data that's needed to render text to the screen.
*/

typedef struct text_s {
	/*! Font to be used. */
	font *fontStyle; 
	/*! The lenght of the text message. */
	unsigned int length;
	/*! Font size in pixels. */
	float size;
	/*! X coordinate of the point where the text is to be rendered (lower left corner). */
	float x;
	/*! Y coordinate of the point where the text is to be rendered (upper left corner). */
	float y; 
	/*! List of meshes to be rendered. */
	linkedList *modelList;
} text;

/*!
	@brief This function is used to create a new text element.
	@param string The message you want to display.
	@param size Font size.
	@param x The X coordinate of the point where the text is to be rendered (lower left corner) of the element.
	@param y The Y coordinate of the point where the text is to be rendered (upper left corner) of the element.
	@return Pointer to the new \a text \a element or NULL if failed.
*/
text* text_create(const char *string, font *F, float size, float x, float y);

void text_update(text *T, const char *string);
void text_draw(text *T, shader *S, mat4x4 projection);

#endif // TEXT_H
