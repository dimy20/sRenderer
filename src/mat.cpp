#include "mat.h"
#include <math.h>

Vec4 Mat4_mult_vec4_project(const Mat4& perspective_proj_mat, const Vec4& world_vertex){
	Vec4 projected_vertex = perspective_proj_mat * world_vertex;

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
