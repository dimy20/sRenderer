#pragma once
#include <cstdint>

struct Fps{
	Fps();
	void cap();
	void counter();
	constexpr uint32_t frames() const{ return m_last_fps; };
	private:
		uint64_t m_prev_time;
		uint32_t m_prev_counter_time;
		uint32_t m_counter;
		uint32_t m_last_fps;
};
