#pragma once
#include <stdint.h>

namespace mate3d{
	struct display{
		display(int _w, int _h);
		void present_pixels(const uint32_t * pixels);
		~display();

		private:
			SDL_Window * m_window;
			SDL_Renderer * m_renderer;
			SDL_Texture * m_fbtexture;
			int m_w;
			int m_h;
	};
};
