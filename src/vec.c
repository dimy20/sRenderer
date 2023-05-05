#include "vec.h"
#include <math.h>

inline Vec3f Vec3f_rotate_z(const Vec3f v, double angle){
	double rotated_x = v.x * cos(angle) - v.y * sin(angle);
	double rotated_y = v.x * sin(angle) + v.y * cos(angle);
	Vec3f rotated = {.x = rotated_x, .y = rotated_y, .z = v.z};
	return rotated;
};

inline Vec3f Vec3f_rotate_x(const Vec3f v, double angle){
	double rotated_y = v.y * cos(angle) - v.z * sin(angle);
	double rotated_z = v.y * sin(angle) + v.z * cos(angle);
	Vec3f rotated = {.x = v.x, .y = rotated_y, .z = rotated_z};
	return rotated;
};

inline Vec3f Vec3f_rotate_y(const Vec3f v, double angle){
	double rotated_x = v.x * cos(angle) - v.z * sin(angle);
	double rotated_z = v.x * sin(angle) + v.z * cos(angle);
	Vec3f rotated = {.x = rotated_x, .y = v.y, .z = rotated_z};
	return rotated;
}
