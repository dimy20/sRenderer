#include "cube.h"
#include <string.h>

static Cube cube;

static Vec3f cube_vertices[CUBE_NUM_VERTICES] = {
	{.x = -1,.y =  -1,.z =  -1},
	{.x = -1,.y =  1, .z = -1},
	{.x = 1, .y =  1, .z = -1},
	{.x = 1, .y = -1, .z = -1},

	{.x = 1,  .y =  1, .z = 1},
	{.x = 1,  .y = -1, .z = 1},
	{.x = -1, .y =  1, .z = 1},
	{.x = -1, .y = -1, .z = 1}
};

static Face cube_faces[CUBE_NUM_FACES] = {
	{.a = 1, .b = 2, .c = 3},
	{.a = 1, .b = 3, .c = 4},

	{.a = 4, .b = 3, .c = 5},
	{.a = 4, .b = 5, .c = 6},

	{.a = 6, .b = 5, .c = 7},
	{.a = 6, .b = 7, .c = 8},

	{.a = 8, .b = 7, .c = 2},
	{.a = 8, .b = 2, .c = 1},

	{.a = 2, .b = 7, .c = 5},
	{.a = 2, .b = 5, .c = 3},

	{.a = 6, .b = 8, .c = 1},
	{.a = 6, .b = 1, .c = 4}
};

const Cube * get_cube(){
	memcpy(cube.vertices, cube_vertices, sizeof(Vec3f) * CUBE_NUM_VERTICES);
	memcpy(cube.faces, cube_faces, sizeof(Face) * CUBE_NUM_FACES);
	return &cube;
}
