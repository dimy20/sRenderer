#include "fps.h"
#include <SDL2/SDL.h>
#include <iostream>

#define TARGET_FPS 60
static const double frame_target_time = 1.0 / static_cast<double>(TARGET_FPS);
static const uint64_t clock_frequency = SDL_GetPerformanceFrequency();

Fps::Fps(){
	m_prev_time = SDL_GetPerformanceCounter();
	m_prev_counter_time = SDL_GetTicks();
	m_counter = 0;
}

void Fps::cap(){
    uint64_t now = SDL_GetPerformanceCounter();
    double elapsed_secs = (now - m_prev_time) / static_cast<double>(clock_frequency);

    if(elapsed_secs < frame_target_time){
    	uint32_t wait_ms = static_cast<uint32_t>((frame_target_time - elapsed_secs) * 1000.0);
    	SDL_Delay(wait_ms);
    }

    m_prev_time = now;
};

void Fps::counter(){
	m_counter++;
    uint32_t now = SDL_GetTicks();
	auto elapsed_ms = now - m_prev_counter_time;

	if(elapsed_ms >= 1000){
		m_last_fps = m_counter;
		m_counter = 0;
		m_prev_counter_time = now;
	}
}
