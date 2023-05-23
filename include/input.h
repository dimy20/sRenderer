#pragma once

#define KEYBOARD_MAX_KEYS 350

#include <SDL2/SDL.h>

struct Input{
	Input();
	void update();
	bool keyboard[KEYBOARD_MAX_KEYS];
	int mouse_x;
	int mouse_y;
	bool quit;
	private:
		void do_keyup(const SDL_KeyboardEvent& e);
		void do_keydown(const SDL_KeyboardEvent& e);
};
