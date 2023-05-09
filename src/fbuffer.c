#include "fbuffer.h"
#include "memory.h"
#include <assert.h>
#include <stdint.h>

Fbuffer * Fbuffer_create(int w, int h){
	Fbuffer * fb = RMALLOC(sizeof(Fbuffer));
	fb->pixels = RMALLOC(sizeof(uint32_t) * w * h);
	fb->w = w;
	fb->h = h;
	return fb;
};

void Fbuffer_destroy(Fbuffer * fb){
	if(fb){
		free(fb->pixels);
		free(fb);
	}
}

inline void Fbuffer_set_pixel(Fbuffer * fb, int x, int y, uint32_t color){
	assert(fb != NULL);
	if(x >= 0 && x < fb->w && y >= 0 && y < fb->h){
		fb->pixels[(fb->h - y - 1) * fb->w + x] = color; // flip y
		//fb->pixels[y * fb->w + x] = color;
	}
}

void Fbuffer_clear(Fbuffer * fb, uint32_t color){
	assert(fb != NULL);
	for(int y = 0; y < fb->h; y++){
		for(int x = 0; x < fb->w; x++){
			Fbuffer_set_pixel(fb, x, y, color);
		}
	}
}

inline uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b){
	return (uint32_t)(r << 24 | g << 16 | b << 8 | 0xff);
}

inline void unpack_color(uint32_t color, uint8_t * r, uint8_t * g, uint8_t * b){
	*r = (color >> 24) & 0xff;
	*g = (color >> 16) & 0xff;
	*b = (color >> 8) & 0xff;
};
