#pragma once
#include <stdint.h>
#include "font.h"

namespace mate3d{
	struct display{
		display(int _w, int _h);
		void put_pixels(const uint32_t * pixels);
		void put_text(const Font& font, const std::string& text);
		void present();
		SDL_Renderer * renderer(){ return m_renderer; };
		~display();

		private:
			SDL_Window * m_window;
			SDL_Renderer * m_renderer;
			SDL_Texture * m_fbtexture;
			int m_w;
			int m_h;
	};
};
