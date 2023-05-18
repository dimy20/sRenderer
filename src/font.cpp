#include <unordered_map>
#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "error.h"
#include "font.h"

static const SDL_Color white = {0xff, 0xff, 0xff};
void mate3d::Font::init_glyph_atlas(SDL_Renderer * renderer){
	SDL_Surface * atlas_surface;
	DIE(!(atlas_surface = SDL_CreateRGBSurface(0, MAX_ATLAS_SIZE, MAX_ATLAS_SIZE, 32, 0, 0, 0, 0xff)), SDL_GetError());

	uint32_t key = SDL_MapRGBA(atlas_surface->format, 0, 0, 0, 0);
	DIE((SDL_SetColorKey(atlas_surface, SDL_TRUE, key) < 0), SDL_GetError());

	int glyph_w, glyph_h;
	int x = 0;
	int y = 0;
	char c[2];

	SDL_Surface * curr_glyph;
	for(char i = 32; i <= 126; i++){
		c[0] = i;
		c[1] = '\0';

		curr_glyph = TTF_RenderUTF8_Blended(m_font, c, white);
		DIE(TTF_SizeText(m_font, c, &glyph_w, &glyph_h) < 0, TTF_GetError());

		if(x + glyph_w >= MAX_ATLAS_SIZE){
			x = 0;
			y += glyph_h + 1; // go down a row
			if(y + glyph_h >= MAX_ATLAS_SIZE){
				std::cerr<< "Failed to create glyph atlas: out of atlas space\n";
				SDL_FreeSurface(curr_glyph);
				goto exit;
			}
		}

		SDL_Rect dest = {x, y, glyph_w, glyph_h};
		DIE((SDL_BlitSurface(curr_glyph, NULL, atlas_surface, &dest) < 0), SDL_GetError());

		m_glyphs[static_cast<int>(i)] = dest;

		SDL_FreeSurface(curr_glyph);
		x += glyph_w;
	}

	DIE(!(m_glyph_atlas = SDL_CreateTextureFromSurface(renderer, atlas_surface)), SDL_GetError());
	exit:
		SDL_FreeSurface(atlas_surface);
}

void mate3d::Font::blit_text(SDL_Renderer * renderer, int x, int y, uint32_t color, const std::string& text) const{
	uint8_t r = static_cast<uint8_t>((color >> 24) & 0xff);
	uint8_t g = static_cast<uint8_t>((color >> 16) & 0xff);
	uint8_t b = static_cast<uint8_t>((color >> 8) & 0xff);

	SDL_SetTextureColorMod(m_glyph_atlas, r, g, b);

	for(auto& c : text){
		assert(m_glyphs.find(c) != m_glyphs.end());
		const SDL_Rect& glyph_src = m_glyphs.at(c);
		SDL_Rect dest = {x, y, glyph_src.w, glyph_src.h};

		SDL_RenderCopy(renderer, m_glyph_atlas, &glyph_src, &dest);
		x += glyph_src.w;
	}
}

mate3d::Font::Font(const std::string& filename, int size, SDL_Renderer * renderer){
	DIE(!(m_font = TTF_OpenFont(filename.c_str(), size)), TTF_GetError());
	init_glyph_atlas(renderer);
}

mate3d::Font::~Font(){
	if(m_font != NULL){
		TTF_CloseFont(m_font);
	}
}
