#pragma once

#include "darray.h"
#include "triangle.h"
#include "vec.h"

typedef struct{
	Vec3 * vertices;
	Face * faces;
	// transform
	Vec3 rotation;
	Vec3 scale;
	Vec3 translation;
}Model;

void Model_destroy(Model * model);
