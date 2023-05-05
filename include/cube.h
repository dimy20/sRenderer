#pragma once

#define CUBE_NUM_VERTICES 8
#define CUBE_NUM_FACES 6 * 2 // 2 triangles per side

#include "vec.h"
#include "triangle.h"

typedef struct{
	Vec3f vertices[CUBE_NUM_VERTICES];
	Face faces[CUBE_NUM_FACES];
}Cube;

const Cube * get_cube();
