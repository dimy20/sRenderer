#pragma once

#include "vec.h"
#include "texture.h"
#include <cstring>

struct Face{
	Face() {};
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
	const Face * face; // keep a pointer to the original face in object space
};

int compare_proj_triangle(const void * _a, const void * _b);
bool comp_proj_triangle(const Proj_triangle& a, const Proj_triangle& b);
