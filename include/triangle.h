#pragma once

#include "vec.h"
#include "fbuffer.h"
#include "texture.h"

typedef struct{
	union{
		struct{
			int a, b, c;
		};
		int indices[3];
	};

	Tex2_coord a_uv;
	Tex2_coord b_uv;
	Tex2_coord c_uv;
	uint32_t color;
}Face;

// after applying transformations and projection to each vertex of a face,
// The resulted projected face or "Proj_triangle" is stored in this struct.
// Which can be used later for rendering as a solid filled triangle or wireframe
typedef struct{
	Vec2 projected_points[3];
	uint32_t color;
	double avg_z;
	const Face * face; // keep a pointer to the original face in object space
}Proj_triangle;

void projected_triangle_draw_filled(Fbuffer * fb, const Proj_triangle * t);
void projected_triangle_draw_wireframe(Fbuffer * fb, const Proj_triangle * t);
int compare_proj_triangle(const void * _a, const void * _b);
