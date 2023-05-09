#include "vec.h"
#include <math.h>

// vec3 operations
inline Vec3 Vec3_rotate_z(const Vec3 v, double angle){
	double rotated_x = v.x * cos(angle) - v.y * sin(angle);
	double rotated_y = v.x * sin(angle) + v.y * cos(angle);
	Vec3 rotated = {.x = rotated_x, .y = rotated_y, .z = v.z};
	return rotated;
};

inline Vec3 Vec3_rotate_x(const Vec3 v, double angle){
	double rotated_y = v.y * cos(angle) - v.z * sin(angle);
	double rotated_z = v.y * sin(angle) + v.z * cos(angle);
	Vec3 rotated = {.x = v.x, .y = rotated_y, .z = rotated_z};
	return rotated;
};

inline Vec3 Vec3_rotate_y(const Vec3 v, double angle){
	double rotated_x = v.x * cos(angle) + v.z * sin(angle);
	double rotated_z = v.x * -sin(angle) + v.z * cos(angle);
	Vec3 rotated = {.x = rotated_x, .y = v.y, .z = rotated_z};
	return rotated;
}

inline Vec3 Vec3_cross(Vec3 a, Vec3 b){
	double x, y, z;
	x = a.y*b.z - a.z*b.y;
	y = a.z*b.x - a.x*b.z;
	z = a.x*b.y - a.y*b.x;
	return (Vec3){.x = x, .y = y, .z = z};
};

inline double Vec3_len(Vec3 v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vec3 Vec3_add(Vec3 a, Vec3 b){
	return (Vec3){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
}

inline Vec3 Vec3_sub(Vec3 a, Vec3 b){
	return (Vec3){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
}

inline Vec3 Vec3_mul(Vec3 a, double t){
	return (Vec3){.x = a.x * t, .y = a.y * t, .z = a.z * t};
}

inline double Vec3_dot(Vec3 a, Vec3 b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vec3 Vec3_normalize(Vec3 v){
	double len = Vec3_len(v);
	return (Vec3){.x = v.x / len,
		           .y = v.y / len,
				   .z = v.z / len };
}

// vec2 operations

inline double Vec2_len(Vec2 v){
	return sqrt(v.x * v.x + v.y * v.y);
}

inline Vec2 Vec2_add(Vec2 a, Vec2 b){
	return (Vec2){.x = a.x + b.x, .y = a.y + b.y};
}

inline Vec2 Vec2_sub(Vec2 a, Vec2 b){
	return (Vec2){.x = a.x - b.x, .y = a.y - b.y};
}

inline Vec2 Vec2_mul(Vec2 a, double t){
	return (Vec2){.x = a.x * t, .y = a.y * t};
};

inline double Vec2_dot(Vec2 a, Vec2 b){
	return a.x*b.x + a.y*b.y;
}

inline Vec2 Vec2_normalize(Vec2 v){
	double len = Vec2_len(v);
	return (Vec2){
		.x = v.x / len,
		.y = v.y / len
	};
}

Vec4 Vec4_from_vec3(Vec3 vec){
	return (Vec4) {.x = vec.x, .y = vec.y, .z = vec.z, 1.0};
}

Vec3 Vec3_from_vec4(Vec4 vec){
	return (Vec3) {.x = vec.x, .y = vec.y, .z = vec.z};
}
