#pragma once

#include "vec.h"
#include <cmath>

struct Mat4{
	double m[4][4];
};

constexpr Mat4 Mat4_id(void){
	return (Mat4){{{1, 0, 0, 0},
				   {0, 1, 0, 0},
				   {0, 0, 1, 0},
				   {0, 0, 0, 1}}};
};

constexpr Mat4 Mat4_scale(const Vec3& s){
	Mat4 mat = Mat4_id();
	mat.m[0][0] = s.x;
	mat.m[1][1] = s.y;
	mat.m[2][2] = s.z;
	return mat;
};

inline Vec4 operator*(const Mat4& mat, const Vec4& vec){
	Vec4 result;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			result.values[i] += mat.m[i][j] * vec.values[j];
		}
	}
	return result;
}

constexpr Mat4 Mat4_translate(const Vec3& t){
	Mat4 result = Mat4_id();
	result.m[0][3] = t.x;
	result.m[1][3] = t.y;
	result.m[2][3] = t.z;
	return result;
};

constexpr Mat4 Mat4_make_rotate_z(double angle){
	Mat4 result = Mat4_id();
	double cos_a = cos(angle);
	double sin_a = sin(angle);

	result.m[0][0] = cos_a;
	result.m[0][1] = -sin_a;
	result.m[1][0] = sin_a;
	result.m[1][1] = cos_a;

	return result;
};

constexpr Mat4 Mat4_make_rotate_x(double angle){
	Mat4 result = Mat4_id();
	double cos_a = cos(angle);
	double sin_a = sin(angle);

	result.m[1][1] = cos_a;
	result.m[1][2] = -sin_a;
	result.m[2][1] = sin_a;
	result.m[2][2] = cos_a;

	return result;
};

constexpr Mat4 Mat4_make_rotate_y(double angle){
	Mat4 result = Mat4_id();
	double cos_a = cos(angle);
	double sin_a = sin(angle);

	result.m[0][0] = cos_a;
	result.m[0][2] = sin_a;
	result.m[2][0] = -sin_a;
	result.m[2][2] = cos_a;

	return result;
};

Mat4 inline operator*(const Mat4& a, const Mat4& b){
	Mat4 result;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			result.m[i][j] = 0;
			for(int k = 0; k < 4; k++){
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return result;
};

constexpr Mat4 Mat4_make_perspective(double fov, double aspect_ratio, double znear, double zfar){
	Mat4 result = {{{0}}};

	result.m[0][0] = aspect_ratio * (1.0 / tan(fov / 2.0));
	result.m[1][1] = 1 / tan(fov / 2.0);
	result.m[2][2] = zfar / (zfar - znear);
	result.m[2][3] = (-zfar * znear) / (zfar - znear);
	result.m[3][2] = 1.0; // keep z

	return result;
};

Vec4 Mat4_mult_vec4_project(const Mat4& perspective_proj_mat, const Vec4& v);
Mat4 look_at(const Vec3& eye, const Vec3& target, const Vec3& up);
