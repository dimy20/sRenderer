#pragma once
#include <memory>
#include <array>
#include "model.h"
#include "vec.h"
#include "triangle.h"

#define MAX_PROJECTED_TRIANGLES 1024 * 10

struct Transform{
	Transform(const Vec3& rot, const Vec3& _scale, const Vec3& trans) : 
		rotation(rot), scale(_scale), translation(trans) {};
	Vec3 rotation;
	Vec3 scale;
	Vec3 translation;
};

struct WorldObj{
	WorldObj(std::unique_ptr<Model>&& _model, const Transform& _transform)
		: model(std::move(_model)), transform(_transform) {};

	// geometry data
	std::unique_ptr<Model> model;

	// transform
	Transform transform;

	// raster data
	std::array<Proj_triangle, MAX_PROJECTED_TRIANGLES> projected_triangles;
	size_t proj_triangles_num = 0;
};
