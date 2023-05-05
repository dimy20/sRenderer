#pragma once

#include <stdint.h>
typedef struct{
	int w;
	int h;
	uint32_t * pixels;
}Fbuffer;

Fbuffer * Fbuffer_create(int w, int h);
void Fbuffer_destroy(Fbuffer * fb);
void Fbuffer_set_pixel(Fbuffer * fb, int x, int y, uint32_t color);
void Fbuffer_clear(Fbuffer * fb, uint32_t color);
uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b);
