#pragma once

#include "fbuffer.h"
#include "vec.h"
#include "texture.h"

void draw_line(Fbuffer * fb, int x1, int y1, int x2, int y2, uint32_t color);
void draw_rect(Fbuffer * fb, int x1, int y1, int w, int h, uint32_t color);
void draw_triangle(Fbuffer * fb, const Vec2 * points, uint32_t color);
void draw_triangle_tex2mapped(Fbuffer * fb, const Vec2 * points, const Tex2_coord * texture_coords, const uint32_t * pixels);
void draw_wireframe_triangle(Fbuffer * fb, const Vec2 * points,  uint32_t color);
