#include "../include/terrain.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <math.h>

terrain *terrain_genDiamondSquare(int size, float range, float factor, int smoothing_passes, const char *texturePath)
{
	terrain *ret = malloc(sizeof(terrain));
	ret->model = NULL;
	ret->heightMap = malloc(sizeof(float) * size * size);
	ret->size = size;

	*((float *)ret->heightMap + 0 * size + 0) = 0;
	*((float *)ret->heightMap + 0 * size + (size - 1)) = 0;
	*((float *)ret->heightMap + (size - 1) * size + 0) = 0;
	*((float *)ret->heightMap + (size - 1) * size + (size - 1)) = 0;

	setRandSeed();
	int i, j, k;

	// NOTE: this is not correct, square values are not using the value of adjacent diamonds correctly
	for (i = size - 1; i >= 2; i /= 2, range *= pow(2, -factor)) {
		for (j = 0; j < size - 1; j += i) {
			for (k = 0; k < size - 1; k += i) {
				float a = access_2df_array(ret->heightMap, size, j, k);
				float b = access_2df_array(ret->heightMap, size, j, k + i);
				float c = access_2df_array(ret->heightMap, size, j + i, k);
				float d = access_2df_array(ret->heightMap, size, j + i, k + i);

				float e = access_2df_array(ret->heightMap, size, j + i / 2, k + i / 2) = (a + b + c + d) / 4.0f + randomValue(range);

				access_2df_array(ret->heightMap, size, j, k + i / 2) =
					(access_2df_array(ret->heightMap, size, j, k) + access_2df_array(ret->heightMap, size, j, k + i) + e) / 3.0f +
					randomValue(range);
				access_2df_array(ret->heightMap, size, j + i / 2, k) =
					(access_2df_array(ret->heightMap, size, j, k) + access_2df_array(ret->heightMap, size, j + i, k) + e) / 3.0f +
					randomValue(range);
				access_2df_array(ret->heightMap, size, j + i, k + i / 2) =
					(access_2df_array(ret->heightMap, size, j + i, k) + access_2df_array(ret->heightMap, size, j + i, k + i) + e) / 3.0f +
					randomValue(range);
				access_2df_array(ret->heightMap, size, j + i / 2, k + i) =
					(access_2df_array(ret->heightMap, size, j, k + i) + access_2df_array(ret->heightMap, size, j + i, k + i) + e) / 3.0f +
					randomValue(range);
			}
		}
	}

	// Post-smoothing: 3x3 box filter, edges clamped to the boundary value.
	// Uses a temp buffer per pass so reads within a pass are not contaminated
	// by writes from earlier cells in the same pass.
	if (smoothing_passes > 0) {
		int pass, r, c, dr, dc;
		for (pass = 0; pass < smoothing_passes; pass++) {
			float *tmp = malloc(sizeof(float) * size * size);
			for (r = 0; r < size; r++) {
				for (c = 0; c < size; c++) {
					float sum = 0.0f;
					for (dr = -1; dr <= 1; dr++) {
						for (dc = -1; dc <= 1; dc++) {
							int rr = r + dr;
							int cc = c + dc;
							if (rr < 0) rr = 0;
							if (cc < 0) cc = 0;
							if (rr > size - 1) rr = size - 1;
							if (cc > size - 1) cc = size - 1;
							sum += access_2df_array(ret->heightMap, size, rr, cc);
						}
					}
					tmp[r * size + c] = sum / 9.0f;
				}
			}
			for (r = 0; r < size; r++)
				for (c = 0; c < size; c++)
					access_2df_array(ret->heightMap, size, r, c) = tmp[r * size + c];
			free(tmp);
		}
	}

	ret->model = malloc(sizeof(mesh));

	mesh_init(ret->model);
	ret->model->indexCount = 6 * (ret->size - 1) * (ret->size - 1);
	ret->model->vertexCount = ret->size * ret->size;

	GLfloat *vertices = malloc(sizeof(float) * ret->model->vertexCount * 8);
	GLuint *indices = malloc(sizeof(GLuint) * ret->model->indexCount);

	// Match the wall/floor coordinate frame: x in [0, size-1], z in
	// [-(size-1), 0]. terrain_heightAt below assumes this mapping.
	for (i = 0, k = 0; i < ret->size; i++)
		for (j = 0; j < ret->size; j++, k += 8) {
			vertices[k] = (float)i;
			vertices[k + 1] = access_2df_array(ret->heightMap, ret->size, i, j);
			vertices[k + 2] = -(float)j;
			vertices[k + 3] = (float)((int)j % 2);
			vertices[k + 4] = (float)((int)i % 2);
			vertices[k + 5] = 0;
			vertices[k + 6] = 1;
			vertices[k + 7] = 0;
			// TODO: fix normals
		}

	for (i = 0, k = 0; i < ret->size - 1; i++) {
		for (j = 0; j < ret->size - 1; j++, k += 6) {
			indices[k] = i + ret->size * j;
			indices[k + 1] = i + ret->size + 1 + ret->size * j;
			indices[k + 2] = i + 1 + ret->size * j;
		}
	}

	for (i = 0, k = 3; i < ret->size - 1; i++) {
		for (j = 0; j < ret->size - 1; j++, k += 6) {
			indices[k] = i + ret->size * j;
			indices[k + 1] = ret->size + i + ret->size * j;
			indices[k + 2] = i + ret->size + 1 + ret->size * j;
		}
	}

	mesh_loadToVAO(ret->model, vertices, indices);
	mesh_textureFromFile(ret->model, texturePath);

	free(indices);
	free(vertices);

	return ret;
}

float terrain_heightAt(terrain *T, float x, float z)
{
	if (T == NULL || T->heightMap == NULL)
		return -INFINITY;

	// World -> grid: x maps to i, -z maps to j (see vertex generation above).
	float fi = x;
	float fj = -z;

	if (fi < 0.0f || fj < 0.0f || fi > (float)(T->size - 1) || fj > (float)(T->size - 1))
		return -INFINITY;

	int i0 = (int)floorf(fi);
	int j0 = (int)floorf(fj);
	int i1 = i0 + 1;
	int j1 = j0 + 1;

	// Clamp the upper corners so a query exactly on the far edge still works.
	if (i1 > T->size - 1) i1 = T->size - 1;
	if (j1 > T->size - 1) j1 = T->size - 1;

	float u = fi - (float)i0;
	float v = fj - (float)j0;

	float h00 = access_2df_array(T->heightMap, T->size, i0, j0);
	float h10 = access_2df_array(T->heightMap, T->size, i1, j0);
	float h01 = access_2df_array(T->heightMap, T->size, i0, j1);
	float h11 = access_2df_array(T->heightMap, T->size, i1, j1);

	float h0 = h00 * (1.0f - u) + h10 * u;
	float h1 = h01 * (1.0f - u) + h11 * u;
	return h0 * (1.0f - v) + h1 * v;
}

void terrain_destroy(terrain *toDestroy)
{
	free(toDestroy->model);
	free(toDestroy->heightMap);
	free(toDestroy);
}
