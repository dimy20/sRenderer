#pragma once

#include <vector>
#include "triangle.h"
#include "vec.h"

struct Model{
	Model() {};
	std::vector<Vec3> vertices;
	std::vector<Face> faces;
	// transform
	Vec3 rotation;
	Vec3 scale;
	Vec3 translation;
};
