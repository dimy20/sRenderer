#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "draw.h"
#include "vec.h"
#include <memory>
#include "fbuffer.h"

Vec3 color_a = {.x = 1.0, .y = 0, .z = 0};
Vec3 color_b = {.x = 0, .y = 1.0, .z = 0};
Vec3 color_c = {.x = 0, .y = 0, .z = 1.0};

extern std::vector<double> zbuffer;

uint32_t interpolate_colors(double alpha, double beta, double gamma){
	Vec3 _color = color_a * alpha + color_b * beta + color_c * gamma;

	uint8_t r = (uint8_t)(_color.x * 0xff);
	uint8_t g = (uint8_t)(_color.y * 0xff);
	uint8_t b = (uint8_t)(_color.z * 0xff);

	return mate3d::pack_color(r, g, b);
}

void draw_line(mate3d::Fbuffer& fb, int x1, int y1, int x2, int y2, uint32_t color){

	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);

	bool transposed = false;

	if(dy > dx){ // transpose the line
		std::swap(x1, y1);
		std::swap(x2, y2);
		std::swap(dx, dy);
		transposed  = true;
	}

	// make sure left to write
	if(x1 > x2){
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	int offset_step = 2 * dy; // avoid use of float
	int offset = 0;
	int y_step = y1 < y2 ? 1 : -1;
	int y = y1;

	if(transposed){
		for(int x = x1; x <= x2; x++){
			fb.set_pixel(y, x, color);
			offset += offset_step;
			if(offset > dx){
				y += y_step;
				offset -= dx * 2;
			}
		}
	}else{
		for(int x = x1; x <= x2; x++){
			fb.set_pixel(x, y, color);
			offset += offset_step;
			if(offset > dx){
				y += y_step;
				offset -= dx * 2;
			}
		}
	}
};

void draw_rect(mate3d::Fbuffer& fb, int x1, int y1, int w, int h, uint32_t color){
   if(w < 0 || h < 0) return;

	if(x1 >= fb.w || y1 >= fb.h || x1 < 0 || y1 < 0){
		return;
	}

	int start_x = x1;
	int end_x = std::min(x1 + w, fb.w - 1);

	int min_y = y1;
	int max_y = std::min(y1 + h, fb.h - 1);

	for(int y = min_y; y <= max_y; y++){
		draw_line(fb, start_x, y, end_x, y, color);
	}
};

static inline double edge(const Vec2f& a, const Vec2f& b, const Vec2f& c){
	// instead of computing the determinant of ab,ac im doing det of ac,ab
	// because i want my function return positive when c is to the right of segment ab.
	// since im using a left handed system im going to be checkig points and edges in a clock-wise
	// order, and i prefer to test for >= 0 instead of <= 0.
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
		*min_x = std::max(std::min(static_cast<int>(points[i].x), *min_x), 0);
		*max_x = std::min(std::max(static_cast<int>(points[i].x), *max_x), max_w - 1);

		*min_y = std::max(std::min(static_cast<int>(points[i].y), *min_y), 0);
		*max_y = std::min(std::max(static_cast<int>(points[i].y), *max_y), max_h - 1);
	}
};

void draw_triangle_tex2mapped(mate3d::Fbuffer& fb, const Vec4 * points, const Tex2_coord * uv_coords, const SDL_Surface * texture){
	assert(texture != NULL);
	assert(points != NULL);
	assert(uv_coords != NULL);

	int min_x, max_x, min_y, max_y;

	Vec2f _points[3] = {points[0].vec2f(), points[1].vec2f(), points[2].vec2f() };
	min_bounding_box(_points, fb.w, fb.h, &min_x, &max_x, &min_y, &max_y);

	auto& a = points[0];
	auto& b = points[1];
	auto& c = points[2];
	Vec2f p;

	// this is actually 2 times the area;
	double p_area = edge(c.vec2f(), a.vec2f(), b.vec2f());

	/* Devide each u and v texture coordinate by it's corresponding w component of the vertex.
	 * This is to do perspective correct interpolation of u and v coordinates.
	 * */
	Vec3 u_over_w(uv_coords[0].u / a.w, uv_coords[1].u / b.w, uv_coords[2].u / c.w);
	Vec3 v_over_w(uv_coords[0].v / a.w, uv_coords[1].v / b.w, uv_coords[2].v / c.w);
	Vec3 inv_ws(1 / a.w, 1 / b.w, 1 / c.w);
	Weights barycentric_weights;
	uint32_t * texture_pixels = reinterpret_cast<uint32_t *>(texture->pixels);

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
			double w0 = edge(b.vec2f(), c.vec2f(), p);
			double w1 = edge(c.vec2f(), a.vec2f(), p);
			double w2 = edge(a.vec2f(), b.vec2f(), p);

			if(w0 >= 0 && w1 >= 0 && w2 >= 0){ // 
				/* Deviding each corresponding parallelogram by the bigger parallelogram
				 * we get the barycentric coordinates in the [0, 1] range.
				 * */
				barycentric_weights.alpha = w0 / p_area;
				barycentric_weights.beta = w1 / p_area;
				barycentric_weights.gamma = 1.0 - barycentric_weights.alpha - barycentric_weights.beta;

				// needed to interpolate z values for pixel x,y:
				// Z values are not linear across the triangle, so we cant linearly interpolate z's,
				// However 1/z is linear, so we can interpolate those and reverse back to z.
				double interpolated_inv_w = dot(inv_ws, barycentric_weights);
				/*
				 * Remember: Z values from camera space are stored in the w component after perspective projection.
				 * The closer w is to the camera, the smaller it will be.
				 * However we interpolate 1/w, so the inverse will happen, the closer W is the bigger 1/w will be.
				 * by doing 1 - 1/w, we flip that back to [closer -> 0, far-> 1.0] instead of (closer -> 1.0, far -> 0)
				 * If the current pixel at (x, y) has an interpolated z depth smaller the the z depth of the previous pixel at
				 * the same (x, y), then it is closer, so replace the color.
				 * */
				if(1.0 - interpolated_inv_w < zbuffer[y * fb.w + x]){ // depth testing
					// perspective correct interpolation.
					double interpolated_u = dot(u_over_w, barycentric_weights);
					double interpolated_v = dot(v_over_w, barycentric_weights);

					interpolated_u /= interpolated_inv_w;
					interpolated_v /= interpolated_inv_w;

					// scale interpolated u and v by texture dimension to get
					// coordinate in texture space.
					int texture_x = static_cast<int>(fabs(interpolated_u) * texture->w);
					int texture_y = static_cast<int>(fabs(interpolated_v) * texture->h);

					uint32_t color = texture_pixels[(texture->h - texture_y - 1) * texture->w + texture_x];
					fb.set_pixel(x, y, color);

					zbuffer[y * fb.w + x] = 1.0 - interpolated_inv_w;
				}

			}
		}
	}

};

/* draws triangle with a flat color, uses same algorithm as above,
   except there is no need to calculate barycentric coordinates 
   */
void draw_triangle(mate3d::Fbuffer& fb, const Vec4 * points, uint32_t color){
	int min_x, max_x, min_y, max_y;

	Vec2f _points[3] = {points[0].vec2f(), points[1].vec2f(), points[2].vec2f() };
	min_bounding_box(_points, fb.w, fb.h, &min_x, &max_x, &min_y, &max_y);

	Vec2f p;
	// A B C
	// 0 1 2
	for(int y = min_y; y <= max_y; y++){
		for(int x = min_x; x <= max_x; x++){
			p.x = x;
			p.y = y;

			double w0 = edge(points[1].vec2f(), points[2].vec2f(), p);
			double w1 = edge(points[2].vec2f(), points[0].vec2f(), p);
			double w2 = edge(points[0].vec2f(), points[1].vec2f(), p);

			if(w0 >= 0 && w1 >= 0 && w2 >= 0){ // 
				fb.set_pixel(x, y, color);
			}
		}
	}

};

void draw_wireframe_triangle(mate3d::Fbuffer& fb, const Vec2f * points,  uint32_t color){
	for(int i = 0; i < 3; i++){
		draw_line(fb, points[i].x, points[i].y,
				      points[(i + 1) % 3].x, points[(i + 1) % 3].y, color);
	};
};
