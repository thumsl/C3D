#include "render.h"

int initOpenGL() {
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if(status != GLEW_OK) {
		fprintf(stderr, "Error intializing GLEW: %s\n", glewGetErrorString(status));
		return 0;
	}
	else {
		printf("GL version: %s\n", glGetString(GL_VERSION));
		return 1;
	}
}
