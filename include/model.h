#pragma once

#include "darray.h"
#include "triangle.h"
#include "vec.h"

typedef struct{
	Vec3f * vertices;
	Face * faces;
	// transform
	Vec3f rotation;
}Model;

void Model_destroy(Model * model);
