#include "../include/level.h"
#include "../include/c3d.h"
#include "../include/mesh.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// Wall geometry covers world x in [x - x_lenght - WALL_WIDTH, x] and world z
// in [-z - z_lenght, -z + 1] (assuming WALL_WIDTH = 1). Pass z_lenght = 0 for
// a horizontal wall that runs in x; pass x_lenght = 0 for a vertical wall
// that runs in z.
mesh *genVertices(int z, int x, int x_lenght, int z_lenght, GLuint *indices, const char *texture_path)
{
	GLfloat xR = x;
	GLfloat xL = x - x_lenght - WALL_WIDTH;
	GLfloat zN = -z + 1;
	GLfloat zF = -z - z_lenght;
	GLfloat h  = WALL_HEIGHT;

	// 16 unique vertices (4 per face × 4 faces): face-aligned normals for
	// correct Phong lighting, with UVs spanning 0→1 across each face so the
	// texture maps once per face without tiling seams (matches the original
	// seamless look on long horizontal-wall faces).
	GLfloat vertices[] = {
		// +X face (east), normal (1, 0, 0)
		xR, 0, zN,   0, 0,    1, 0, 0,   //  0
		xR, 0, zF,   1, 0,    1, 0, 0,   //  1
		xR, h, zF,   1, 1,    1, 0, 0,   //  2
		xR, h, zN,   0, 1,    1, 0, 0,   //  3

		// -X face (west), normal (-1, 0, 0)
		xL, 0, zF,   0, 0,   -1, 0, 0,   //  4
		xL, 0, zN,   1, 0,   -1, 0, 0,   //  5
		xL, h, zN,   1, 1,   -1, 0, 0,   //  6
		xL, h, zF,   0, 1,   -1, 0, 0,   //  7

		// +Z face (near), normal (0, 0, 1)
		xR, 0, zN,   0, 0,    0, 0, 1,   //  8
		xL, 0, zN,   1, 0,    0, 0, 1,   //  9
		xL, h, zN,   1, 1,    0, 0, 1,   // 10
		xR, h, zN,   0, 1,    0, 0, 1,   // 11

		// -Z face (far), normal (0, 0, -1)
		xL, 0, zF,   0, 0,    0, 0, -1,  // 12
		xR, 0, zF,   1, 0,    0, 0, -1,  // 13
		xR, h, zF,   1, 1,    0, 0, -1,  // 14
		xL, h, zF,   0, 1,    0, 0, -1,  // 15

		// +Y face (top), normal (0, 1, 0)
		xR, h, zN,   0, 0,    0, 1, 0,   // 16
		xR, h, zF,   0, 1,    0, 1, 0,   // 17
		xL, h, zF,   1, 1,    0, 1, 0,   // 18
		xL, h, zN,   1, 0,    0, 1, 0,   // 19
	};

	mesh *model = malloc(sizeof(mesh));
	mesh_init(model);
	model->indexCount = 30;
	model->vertexCount = 20;

	mesh_loadToVAO(model, vertices, indices);
	mesh_textureFromFile(model, texture_path);

	return model;
}

static bool pixel_is_wall(const unsigned char *pixels, int size, int r, int c)
{
	int idx = (r * size + c) * 3;
	int rgb = pixels[idx] | (pixels[idx + 1] << 8) | (pixels[idx + 2] << 16);
	return rgb == WALL;
}

level *level_loadMeshes(const char *path, const char *texture_path)
{
	SDL_Surface *map = IMG_Load(path);

	if (map == NULL) {
		// TODO: IMG_Load return on fail is not null?
		fprintf(stderr, "Failed to load level %s\n", path);
		return NULL;
	}

	level *ret = malloc(sizeof(level));
	ret->size = map->w;
	ret->meshList = list_create();
	int size = (int)ret->size;
	const unsigned char *pixels = (const unsigned char *)map->pixels;

	GLuint indices[] = {  0,  1,  3,   1,  2,  3,    // +X face
			      6,  4,  5,   6,  7,  4,    // -X face
			     11,  9,  8,  11, 10,  9,    // +Z face
			     13, 12, 14,  12, 15, 14,    // -Z face
			     16, 17, 19,  17, 18, 19 };  // +Y face (top)

	bool *visited = calloc((size_t)size * size, sizeof(bool));

	// Pass 1: merge horizontal runs of length ≥ 2 into single elongated
	// meshes. Singletons are deferred to pass 2 so column-aligned wall
	// pixels become one elongated vertical mesh instead of N separate
	// meshes (which would have a visible texture seam at every boundary).
	for (int r = 0; r < size; r++) {
		int run_start = -1;
		for (int c = 0; c <= size; c++) {
			bool is_wall = (c < size) && pixel_is_wall(pixels, size, r, c);
			if (is_wall && run_start < 0)
				run_start = c;
			else if (!is_wall && run_start >= 0) {
				int len = c - run_start;
				if (len >= 2) {
					int last_col = c - 1;
					list_insert(ret->meshList, genVertices(r, last_col, len, 0, indices, texture_path));
					for (int cc = run_start; cc < c; cc++)
						visited[r * size + cc] = true;
				}
				run_start = -1;
			}
		}
	}

	// Pass 2: merge vertical runs (any length) of unvisited wall pixels.
	for (int c = 0; c < size; c++) {
		int run_start = -1;
		for (int r = 0; r <= size; r++) {
			bool eligible = (r < size) && !visited[r * size + c] && pixel_is_wall(pixels, size, r, c);
			if (eligible && run_start < 0)
				run_start = r;
			else if (!eligible && run_start >= 0) {
				int len = r - run_start;
				list_insert(ret->meshList, genVertices(run_start, c, 0, len, indices, texture_path));
				run_start = -1;
			}
		}
	}

	free(visited);
	SDL_FreeSurface(map);

	return ret;
}

float level_groundHeightAt(level *L, float x, float z, float max_y)
{
	if (L == NULL || L->meshList == NULL)
		return -INFINITY;

	float best = -INFINITY;
	node *n = L->meshList->head;
	while (n) {
		mesh *m = (mesh *)n->data;
		if (m != NULL && m->hitbox != NULL) {
			boundingBox *hb = m->hitbox;
			if (x >= hb->min[0] && x <= hb->max[0] &&
			    z >= hb->min[2] && z <= hb->max[2]) {
				float top = hb->max[1];
				if (top <= max_y && top > best)
					best = top;
			}
		}
		n = n->next;
	}
	return best;
}
