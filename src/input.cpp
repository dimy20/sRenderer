#include "input.h"
#include <iostream>

void Input::do_keyup(const SDL_KeyboardEvent& e){
	if (e.repeat == 0 && e.keysym.scancode < KEYBOARD_MAX_KEYS){
		keyboard[e.keysym.scancode] = false;
	}
}

void Input::do_keydown(const SDL_KeyboardEvent& e){
	if (e.repeat == 0 && e.keysym.scancode < KEYBOARD_MAX_KEYS){
		keyboard[e.keysym.scancode] = true;
	}
}

Input::Input(){
	std::fill(keyboard, keyboard + KEYBOARD_MAX_KEYS, false); 
	quit = false;
};

void Input::update(){
	SDL_Event e;
	SDL_GetGlobalMouseState(&mouse_x, &mouse_y);

	while(SDL_PollEvent(&e)){
		switch(e.type){
			case SDL_QUIT: quit = true; break;
			case SDL_KEYDOWN: do_keydown(e.key); break;
			case SDL_KEYUP: do_keyup(e.key); break;
			default: break;
		};
	};
}
