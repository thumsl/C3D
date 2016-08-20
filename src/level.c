#include "../include/level.h"
#include "../include/engine.h"
#include "../include/mesh.h"
#include <SDL2/SDL_image.h>

mesh* genVertices(int z, int x, int lenght) {
	GLfloat vertices[] = {
		x,						0,				-z + 1,		0,		0,		 1,	0,  1,
		x,						0,				-z,			1,		0,		 1,	0, -1,
		x-lenght-WALL_WIDTH,	0,				-z,			0,		0,		-1,	0, -1,
		x-lenght-WALL_WIDTH,	0,				-z + 1,		1,		0,		-1,	0,  1,
		x,						WALL_HEIGHT,	-z + 1,		0,		1,		 1,	0,  1,
		x,						WALL_HEIGHT,	-z,			1,		1,		 1,	0, -1,
		x-lenght-WALL_WIDTH,	WALL_HEIGHT,	-z,			0,		1,		-1,	0, -1,
		x-lenght-WALL_WIDTH,	WALL_HEIGHT,	-z + 1,		1,		1,		-1,	0,  1
	};

	GLuint indices[] = {
		0, 1, 4,
		1, 5, 4,
		7, 2, 3,
		7, 6, 2,
		4, 3, 0,
		4, 7, 3,
		1, 2, 5,
		2, 6, 5
	};

	mesh* model = (struct mesh*)malloc(sizeof(mesh));
	mesh_init(model);
	model->indexCount = 24;
	model->vertexCount = 8;

	mesh_loadToVAO(model, vertices, indices);
	mesh_textureFromFile(model, "res/textures/steel.jpg");

    return model;
}

level* level_loadMeshes(const char *path) {
	SDL_Surface *map = IMG_Load(path);

	if (map == NULL) {
		// TODO: IMG_Load return on fail is not null?
		fprintf(stderr, "Failed to load level %s\n", path);
		return NULL;
	}

	level *ret = (level*)malloc(sizeof(level));
	ret->size = map->w;
	ret->meshList = list_create();

	unsigned int i, j, z, c;
	for (i = 0, j = 1, c = 1; i < 3 * map->w * map->h; i+=3, j++) {
		//printf("Count = %d, J = %d\n", count++ + 1, j);
		int x = ((char*)map->pixels)[i] | ((char*)map->pixels)[i+1] << 8 | ((char*)map->pixels)[i+2] << 16;

		if (x == WALL && j + 1 < ret->size) {
			int next = ((char*)map->pixels)[i+3] | ((char*)map->pixels)[i+4] << 8 | ((char*)map->pixels)[i+5] << 16;
			if (next == WALL)
				c++;
			else {
				list_insert(ret->meshList, genVertices(i/(ret->size * 3), j-1, c));
				//printf("1. Wall starts at %d, ends at %d. Z = %d\n", j-1-c, j-1, i/(ret->size * 3));
				c = 1;
			}
		}
		else if (x == WALL) {
			//printf("2. Wall starts at %d, ends at %d. Z = %d\n", j-1-c, j-1, i/(ret->size * 3));
			list_insert(ret->meshList, genVertices(i/(ret->size * 3), j-1, c));
			c = 1;
		}

		if (j == ret->size)
			j = 0;
	}

	SDL_FreeSurface(map);

	return ret;
}