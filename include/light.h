#pragma once

#include "vec.h"
#include <stdint.h>

// global light
typedef struct{
	Vec3 direction;
	uint32_t color;
}Light;

extern Light light;
