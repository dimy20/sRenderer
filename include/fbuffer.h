#pragma once

#include <cstdint>
#include <vector>



namespace mate3d{
	struct Fbuffer{
		Fbuffer(int _w, int _h) : w(_w), h(_h) { pixels.resize(w * h, 0); };
		inline void set_pixel(int x, int y, uint32_t color){
			if(x >= 0 && x < w && y >= 0 && y < h){
				pixels[(h - y - 1) * w + x] = color; // flip y
			}
		}
		inline void clear(uint32_t color) { std::fill(pixels.begin(), pixels.end(), color); };

		int w;
		int h;
		std::vector<uint32_t> pixels;
	};

	constexpr uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b){
		return static_cast<uint32_t>(r << 24 | g << 16 | b << 8 | 0xff);
	}

	constexpr void unpack_color(uint32_t color, uint8_t * r, uint8_t * g, uint8_t * b){
		*r = static_cast<uint8_t>((color >> 24) & 0xff);
		*g = static_cast<uint8_t>((color >> 16) & 0xff);
		*b = static_cast<uint8_t>((color >> 8) & 0xff);
	}
};
