#include "vec.h"
#include "mat.h"

Vec2f weak_project(const Vec3& p, double fov_factor){
	return Vec2f((p.x * fov_factor) / p.z, (p.y * fov_factor) / p.z);
};

Vec4 perspective_project_vertex(const Mat4& perspective_matrix, const Vec4& transformed_vertex){
	Vec4 projected_vertex = perspective_matrix * transformed_vertex;

	/* Perform perspective division:
	   original Z depth, World z value of v is kept in W component of 
	   projected_v, we can use this value to perform perspective division. */
	if(projected_vertex.w != 0.0){
		projected_vertex.x /= projected_vertex.w;
		projected_vertex.y /= projected_vertex.w;
		projected_vertex.z /= projected_vertex.w;
	};

	return projected_vertex;
};
