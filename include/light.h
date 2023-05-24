#pragma once

#include "vec.h"
#include <stdint.h>

// global light
struct Light{
	Light(const Vec3& _direction, uint32_t _color) : direction(_direction), color(_color) {};
	Vec3 direction;
	uint32_t color;
};

extern Light light;
uint32_t light_color_apply_intensity(uint32_t light_color, float intensity);
