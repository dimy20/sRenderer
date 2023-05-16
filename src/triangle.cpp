#include "triangle.h"
#include "primitives2d.h"
#include <stdlib.h>

int compare_proj_triangle(const void * _a, const void * _b){
	Proj_triangle * a = (Proj_triangle *)_a;
	Proj_triangle * b = (Proj_triangle *)_b;
	return (b->avg_z > a->avg_z) - (b->avg_z < a->avg_z);
};

bool comp_proj_triangle(const Proj_triangle& a, const Proj_triangle& b){ return b.avg_z > a.avg_z; }
