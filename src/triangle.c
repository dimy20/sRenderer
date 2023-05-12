#include "triangle.h"
#include "primitives2d.h"
#include <stdlib.h>

inline int compare_proj_triangle(const void * _a, const void * _b){
	Proj_triangle * a = (Proj_triangle *)_a;
	Proj_triangle * b = (Proj_triangle *)_b;
	return (b->avg_z > a->avg_z) - (b->avg_z < a->avg_z);
};
