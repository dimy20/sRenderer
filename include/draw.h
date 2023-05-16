#pragma once

#include "vec.h"
#include "texture.h"
#include "fbuffer.h"

void draw_line(mate3d::Fbuffer& fb, int x1, int y1, int x2, int y2, uint32_t color);
void draw_rect(mate3d::Fbuffer& fb, int x1, int y1, int w, int h, uint32_t color);
void draw_triangle(mate3d::Fbuffer& fb, const Vec4 * points, uint32_t color);
void draw_triangle_tex2mapped(mate3d::Fbuffer& fb, const Vec4 * points, const Tex2_coord * texture_coords, const SDL_Surface * texture);
void draw_wireframe_triangle(mate3d::Fbuffer& fb, const Vec4 * points,  uint32_t color);
