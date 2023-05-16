#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "primitives2d.h"
#include "vec.h"

#define SWAP(type, a, b) do { \
    type temp = a; \
    a = b; \
    b = temp; \
} while (0)

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

Vec3 color_a = {.x = 1.0, .y = 0, .z = 0};
Vec3 color_b = {.x = 0, .y = 1.0, .z = 0};
Vec3 color_c = {.x = 0, .y = 0, .z = 1.0};

uint32_t interpolate_colors(double alpha, double beta, double gamma){
	Vec3 _color = color_a * alpha + color_b * beta + color_c * gamma;

	uint8_t r = (uint8_t)(_color.x * 0xff);
	uint8_t g = (uint8_t)(_color.y * 0xff);
	uint8_t b = (uint8_t)(_color.z * 0xff);

	return pack_color(r, g, b);
}

void draw_line(Fbuffer * fb, int x1, int y1, int x2, int y2, uint32_t color){

	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);

	bool transposed = false;

	if(dy > dx){ // transpose the line
		SWAP(int, x1, y1);
		SWAP(int, x2, y2);
		SWAP(int, dx, dy);
		transposed  = true;
	}

	// make sure left to write
	if(x1 > x2){
		SWAP(int, x1, x2);
		SWAP(int, y1, y2);
	}

	int offset_step = 2 * dy; // avoid use of float
	int offset = 0;
	int y_step = y1 < y2 ? 1 : -1;
	int y = y1;

	if(transposed){
		for(int x = x1; x <= x2; x++){
			Fbuffer_set_pixel(fb, y, x, color);
			offset += offset_step;
			if(offset > dx){
				y += y_step;
				offset -= dx * 2;
			}
		}
	}else{
		for(int x = x1; x <= x2; x++){
			Fbuffer_set_pixel(fb, x, y, color);
			offset += offset_step;
			if(offset > dx){
				y += y_step;
				offset -= dx * 2;
			}
		}
	}
};

void draw_rect(Fbuffer * fb, int x1, int y1, int w, int h, uint32_t color){
   if(w < 0 || h < 0) return;

	if(x1 >= fb->w || y1 >= fb->h || x1 < 0 || y1 < 0){
		return;
	}

	int start_x = x1;
	int end_x = MIN(x1 + w, fb->w - 1);

	int min_y = y1;
	int max_y = MIN(y1 + h, fb->h - 1);

	for(int y = min_y; y <= max_y; y++){
		draw_line(fb, start_x, y, end_x, y, color);
	}
};

static inline double edge(const Vec2f& a, const Vec2f& b, const Vec2f& c){
	// instead of computing the determinant of ab,ac im doing det of ac,ab
	// because i want my function return positive when c is to the right of segment ab.
	// since im using a left handed system im going to be checkig points and edges in a clock-wise
	// order, and i prefer to test for >= 0 instead of <= 0.
	//Vec2 ab = Vec2_sub(*b, *a);
	//Vec2 ac = Vec2_sub(*c, *a);
	Vec2f ab = b - a;
	Vec2f ac = c - a;
	return ac.x * ab.y - ab.x * ac.y;
};

static inline void min_bounding_box(const Vec2f * points, int max_w, int max_h, int * min_x, int * max_x, int * min_y, int * max_y){
	*min_x = max_w - 1;
	*max_x = 0;

	*min_y = max_h - 1;
	*max_y = 0;

	for(int i = 0; i < 3; i++){
		*min_x = MAX(MIN(points[i].x, *min_x), 0);
		*max_x = MIN(MAX(points[i].x, *max_x), max_w - 1);

		*min_y = MAX(MIN(points[i].y, *min_y), 0);
		*max_y = MIN(MAX(points[i].y, *max_y), max_h - 1);
	}
};

void draw_triangle_tex2mapped(Fbuffer * fb, const Vec2f * points, const Tex2_coord * uv_coords, const uint32_t * texture_pixels){
	int min_x, max_x, min_y, max_y;
	min_bounding_box(points, fb->w, fb->h, &min_x, &max_x, &min_y, &max_y);

	const Vec2f& a = points[0];
	const Vec2f& b = points[1];
	const Vec2f& c = points[2];
	Vec2f p;

	// this is actually 2 times the area;
	double p_area = edge(c, a, b);

	if(uv_coords == NULL || texture_pixels == NULL){
		//TODO: error
		return;
	}

	for(int y = min_y; y <= max_y; y++){
		for(int x = min_x; x <= max_x; x++){
			p.x = x;
			p.y = y;
			/* Point P is inside the triangle abc if the point P is to the right of all 3 edges
			 * clock-wise winding order
			 *       B
			 *        \
			 *    ^    
			 *   /  p
			 *  A     <- C
			 * */
			double w0 = edge(b, c, p);
			double w1 = edge(c, a, p);
			double w2 = edge(a, b, p);

			if(w0 >= 0 && w1 >= 0 && w2 >= 0){ // 
				double alpha = w0 / p_area;
				double beta = w1 / p_area;
				double gamma = 1.0 - alpha - beta;

				// interpolate texture offset using barycentric coordinates
				int texture_x = (int)(fabs(uv_coords[0].u * alpha + uv_coords[1].u * beta + uv_coords[2].u * gamma) * 64);
				int texture_y = (int)(fabs(uv_coords[0].v * alpha + uv_coords[1].v * beta + uv_coords[2].v * gamma) * 64);

				uint32_t color = texture_pixels[(64 - texture_y - 1) * 64 + texture_x];

				Fbuffer_set_pixel(fb, x, y, color);
			}
		}
	}

};

/* draws triangle with a flat color, uses same algorithm as above,
   except there is no need to calculate barycentric coordinates 
   */
void draw_triangle(Fbuffer * fb, const Vec2f * points, uint32_t color){
	int min_x, max_x, min_y, max_y;
	min_bounding_box(points, fb->w, fb->h, &min_x, &max_x, &min_y, &max_y);

	Vec2f p;
	// A B C
	// 0 1 2
	for(int y = min_y; y <= max_y; y++){
		for(int x = min_x; x <= max_x; x++){
			p.x = x;
			p.y = y;

			double w0 = edge(points[1], points[2], p);
			double w1 = edge(points[2], points[0], p);
			double w2 = edge(points[0], points[1], p);

			if(w0 >= 0 && w1 >= 0 && w2 >= 0){ // 
				Fbuffer_set_pixel(fb, x, y, color);
			}
		}
	}

};

void draw_wireframe_triangle(Fbuffer * fb, const Vec2f * points,  uint32_t color){
	for(int i = 0; i < 3; i++){
		draw_line(fb, points[i].x, points[i].y,
				      points[(i + 1) % 3].x, points[(i + 1) % 3].y, color);
	};
};
