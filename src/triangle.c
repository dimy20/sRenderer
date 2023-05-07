#include "triangle.h"
#include "primitives2d.h"

void projected_triangle_draw_filled(Fbuffer * fb, const Proj_triangle * t){
	const Vec2f * points = t->projected_points;
	draw_triangle(fb, points[0].x, points[0].y, 
				 points[1].x, points[1].y, 
				 points[2].x, points[2].y, 0xffffffff);
};

void projected_triangle_draw_wireframe(Fbuffer * fb, const Proj_triangle * t){
	for(int i = 0; i < 3; i++){
		const Vec2f * v1 = &t->projected_points[i];
		const Vec2f * v2 = &t->projected_points[(i + 1) % 3];
		draw_line(fb, v1->x, v1->y, v2->x, v2->y, 0xff00ffff);
	}
}
