#pragma once

#include "vec.h"
#include "triangle.h"
#include "mat.h"

namespace pipeline{
	bool backface_culled(const Vec3 * transformed_vertices, Vec3 * normal);
	void transformations(const Face& face, Vec3 * transformed_vertices, const Mat4& world_matrix);
	void projection(const Vec3 * transformed_vertices, Proj_triangle& projected_triangle);
};




