#include "../include/c3d.h"

bool initOpenGL() {
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if(status != GLEW_OK) {
		fprintf(stderr, "Error intializing GLEW: %s\n", glewGetErrorString(status));
		return 0;
	}
	else {
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		printf("GL version: %s\n", glGetString(GL_VERSION));
		return 1;
	}
}

void c3d_quit() {
	SDL_Quit();
	exit(EXIT_FAILURE);
}
