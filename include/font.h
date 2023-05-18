#pragma once

#include <cstdint>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>

#define MAX_GLYPHS 128
#define MAX_ATLAS_SIZE 512

namespace mate3d{
	struct Font{
		Font() {};
		Font(const std::string& filename, int size, SDL_Renderer * renderer);
		~Font();
		SDL_Texture * to_texture(SDL_Renderer * renderer, const std::string& text, uint32_t color, int * w, int * h) const;
		void blit_text(SDL_Renderer * renderer, int x, int y, uint32_t color, const std::string& text) const;
		private:
			void init_glyph_atlas(SDL_Renderer * renderer);
			TTF_Font * m_font = NULL;
			std::unordered_map<char, SDL_Rect> m_glyphs;
			SDL_Texture * m_glyph_atlas;
	};
}
