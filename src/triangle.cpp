#include "triangle.h"
#include <stdlib.h>

bool comp_proj_triangle(const Proj_triangle& a, const Proj_triangle& b){
	return b.avg_z > a.avg_z; 
}
