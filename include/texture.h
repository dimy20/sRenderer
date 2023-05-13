#pragma once

#include <stdint.h>
#include <SDL2/SDL_image.h>
typedef struct{ 
	double u, v;
}Tex2_coord;

extern int texture_width;
extern int texture_height;
extern uint32_t * sample_texture;
SDL_Surface * load_texture(const char * filename);
