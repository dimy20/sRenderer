#pragma once

#include "vec.h"
#include "fbuffer.h"

typedef struct{
	union{
		struct{
			int a, b, c;
		};
		int indices[3];
	};
}Face;

// after applying transformations and projection to each vertex of a face,
// The resulted projected face or "Proj_triangle" is stored in this struct.
// Which can be used later for rendering as a solid filled triangle or wireframe
typedef struct{
	Vec2f projected_points[3];
}Proj_triangle;

void projected_triangle_draw_filled(Fbuffer * fb, const Proj_triangle * t);
void projected_triangle_draw_wireframe(Fbuffer * fb, const Proj_triangle * t);
