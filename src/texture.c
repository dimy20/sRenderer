#include <stdio.h>
#include "texture.h"
#include <SDL2/SDL_image.h>
#include "error.h"

SDL_Surface * load_texture(const char * filename){
	SDL_Surface * s, * s1;
	
	DIE(!(s = IMG_Load(filename)), IMG_GetError());
	DIE(!(s1 = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_RGBA8888, 0)), IMG_GetError());

	SDL_FreeSurface(s);
	return s1;
};
