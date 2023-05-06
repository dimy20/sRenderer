#include "vec.h"
#include <math.h>

// vec3 operations
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

inline Vec3f Vec3f_cross(Vec3f a, Vec3f b){
	double x, y, z;
	x = a.y*b.z - a.z*b.y;
	y = a.z*b.x - a.x*b.z;
	z = a.x*b.y - a.y*b.x;
	return (Vec3f){.x = x, .y = y, .z = z};
};

inline double Vec3f_len(Vec3f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vec3f Vec3f_add(Vec3f a, Vec3f b){
	return (Vec3f){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
}

inline Vec3f Vec3f_sub(Vec3f a, Vec3f b){
	return (Vec3f){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
}

inline Vec3f Vec3f_mul(Vec3f a, double t){
	return (Vec3f){.x = a.x * t, .y = a.y * t, .z = a.z * t};
}

inline double Vec3f_dot(Vec3f a, Vec3f b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

// vec2 operations

inline double Vec2f_len(Vec2f v){
	return sqrt(v.x * v.x + v.y * v.y);
}

inline Vec2f Vec2f_add(Vec2f a, Vec2f b){
	return (Vec2f){.x = a.x + b.x, .y = a.y + b.y};
}

inline Vec2f Vec2f_sub(Vec2f a, Vec2f b){
	return (Vec2f){.x = a.x - b.x, .y = a.y - b.y};
}

inline Vec2f Vec2f_mul(Vec2f a, double t){
	return (Vec2f){.x = a.x * t, .y = a.y * t};
};

inline double Vec2f_dot(Vec2f a, Vec2f b){
	return a.x*b.x + a.y*b.y;
}
