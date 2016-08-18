#include "../include/level.h"
#include "../include/mesh.h"
#include <SDL2/SDL_image.h>

mesh* genVertices(int z, int x, int lenght) {
	float width = 1;
	float height = 4;

	GLfloat vertices[] = {
		x,			0,		z - width,	0, 1, -1, 0, 0,
		x,			height,	z - width,	1, 0, -1, 0, 0,
		x,			height,	z,			0, 1, -1, 0, 0,
		x,			0,		z,			1, 0, -1, 0, 0,
		x + lenght,	height,	z,			0, 1,  1, 0, 0,
		x + lenght, 0,		z,			1, 0,  1, 0, 0,
		x + lenght,	0,		z - width,	0, 1,  1, 0, 0,
		x + lenght,	height,	z - width,	1, 0,  1, 0, 0,
	};

	GLuint indices[] = {
		0, 3, 1,
		3, 2, 1,
		1, 6, 0,
		1, 7, 6, 
		7, 5, 6,
		7, 4, 5,
		3, 5, 2,
		5, 4, 2
	};

	mesh* model = (struct mesh*)malloc(sizeof(mesh));
	mesh_init(model);
	model->indexCount = 24;
	model->vertexCount = 8;

	mesh_loadToVAO(model, vertices, indices);
	mesh_textureFromFile(model, "res/textures/grass.jpg");

    return model;	
}

level* level_loadMeshes(const char *path) {
	SDL_Surface *map = IMG_Load(path);

	if (map == NULL) {
		fprintf(stderr, "Failed to load level %s\n", path);
		return NULL;
	}

	level *ret = (level*)malloc(sizeof(level));
	ret->size = map->w;
	ret->meshList = list_create();
	if (ret->meshList->head == NULL)
		printf("yep empty\n");

	unsigned int i, j, z, c = 0;
	for (i = 0, j = 1; i < 3 * ret->size * ret->size; i+=3, j++) {
		
		int x = ((char*)map->pixels)[i] | ((char*)map->pixels)[i+1] << 8 | ((char*)map->pixels)[i+2] << 16;

		if (x == WALL && j + 1 < ret->size) {
			int next = ((char*)map->pixels)[i+3] | ((char*)map->pixels)[i+4] << 8 | ((char*)map->pixels)[i+5] << 16;
			if (next == WALL)
				c++;
			else {
				list_insert(ret->meshList, genVertices(i/(ret->size * 3)-1, j-1, c));
				c = 0;
			}
		}
		else if (x == WALL) {
			list_insert(ret->meshList, genVertices(i/(ret->size * 3)-1, j-1, c));
			c = 0;
		}

		if (j == ret->size)
			j = 0;
	}

	SDL_FreeSurface(map);

	return ret;
}