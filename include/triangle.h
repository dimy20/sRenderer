#pragma once

#include "vec.h"

typedef struct{
	union{
		struct{
			int a, b, c;
		};
		int indices[3];
	};
}Face;

typedef struct{
	Vec2f projected_points[3];
}Triangle;
