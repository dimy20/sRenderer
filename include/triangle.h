#pragma once

#include "vec.h"
#include "texture.h"
#include <cstring>

struct Face{
	Face() {};
	/*
	Face(std::initializer_list<int> _indices,  Tex2_coord _a_uv, Tex2_coord _b_uv, Tex2_coord _c_uv){
		std::copy(_indices.begin(), _indices.end(), indices);
		uv_coords = _a_uv;
		uv_coords = _b_uv;
		uv_coords = _c_uv;
	};
	*/
	union{
		struct{
			int a, b, c;
		};
		int indices[3];
	};
	int uv_indices[3]; // a_uv, b_uv, c_uv;
	int normal_indices[3];
	uint32_t color;
};

// after applying transformations and projection to each vertex of a face,
// The resulted projected face or "Proj_triangle" is stored in this struct.
// Which can be used later for rendering as a solid filled triangle or wireframe
struct Proj_triangle{
	Vec4 projected_points[3];
	uint32_t color;
	double avg_z;
	const Face * face; // keep a pointer to the original face in object space
};

int compare_proj_triangle(const void * _a, const void * _b);
bool comp_proj_triangle(const Proj_triangle& a, const Proj_triangle& b);
