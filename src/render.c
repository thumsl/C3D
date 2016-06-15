#include "../include/render.h"

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

	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

}
