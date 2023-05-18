
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <assert.h>

#include "error.h"
#include "display.h"
#include "font.h"

static void D_Init(int window_w, int window_h,
				   SDL_Window ** window,
				   SDL_Renderer ** renderer,
				   SDL_Texture ** fb_texture){

	DIE((SDL_Init(SDL_INIT_VIDEO) < 0), SDL_GetError());

	*window = SDL_CreateWindow("3d Renderer",
				              SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED,
							  window_w,
							  window_h,
							  0);

	DIE(!*window, SDL_GetError());
	DIE(!(*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_PRESENTVSYNC)), SDL_GetError());

	*fb_texture = SDL_CreateTexture(*renderer,
									SDL_PIXELFORMAT_RGBA8888,
									SDL_TEXTUREACCESS_STREAMING,
									window_w,
									window_h);
	DIE(!*fb_texture, SDL_GetError());
};

mate3d::display::display(int _w, int _h) : m_w(_w), m_h(_h) {
	D_Init(_w, _h, &m_window, &m_renderer, &m_fbtexture);
};

mate3d::display::~display(){
	SDL_DestroyTexture(m_fbtexture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void mate3d::display::put_pixels(const uint32_t * pixels){
	assert(pixels != NULL);
	int pitch = m_w * sizeof(uint32_t);
	DIE((SDL_UpdateTexture(m_fbtexture, NULL, pixels, pitch) < 0), SDL_GetError());
	DIE((SDL_RenderCopy(m_renderer, m_fbtexture, NULL, NULL) < 0), SDL_GetError());
};

void mate3d::display::put_text(const mate3d::Font& font, const std::string& text){
	font.blit_text(m_renderer, 0, 0, 0xff0000ff, text);
};

void mate3d::display::present(){ SDL_RenderPresent(m_renderer); }
