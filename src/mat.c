#include "mat.h"
#include <math.h>

inline Mat4 Mat4_id(void){
	Mat4 id;
	for(int i =0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			id.m[i][j] = j == i ? 1 : 0;
		}
	}
	return id;
};

inline Mat4 Mat4_scale(double sx, double sy, double sz){
	Mat4 mat = Mat4_id();
	mat.m[0][0] = sx;
	mat.m[1][1] = sy;
	mat.m[2][2] = sz;
	return mat;
};

inline Vec4 Mat4_mult_vec4(Mat4 mat, Vec4 vec){
	Vec4 result = {.x = 0, .y = 0, .z = 0, .w =  0};
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			result.values[i] += mat.m[i][j] * vec.values[j];
		}
	}
	return result;
};

inline Mat4 Mat4_translate(double tx, double ty, double tz){
	Mat4 result = Mat4_id();
	result.m[0][3] = tx;
	result.m[1][3] = ty;
	result.m[2][3] = tz;
	return result;
};

inline Mat4 Mat4_make_rotate_z(double angle){
	Mat4 result = Mat4_id();
	double cos_a = cos(angle);
	double sin_a = sin(angle);

	result.m[0][0] = cos_a;
	result.m[0][1] = -sin_a;
	result.m[1][0] = sin_a;
	result.m[1][1] = cos_a;

	return result;
};

inline Mat4 Mat4_make_rotate_x(double angle){
	Mat4 result = Mat4_id();
	double cos_a = cos(angle);
	double sin_a = sin(angle);

	result.m[1][1] = cos_a;
	result.m[1][2] = -sin_a;
	result.m[2][1] = sin_a;
	result.m[2][2] = cos_a;

	return result;
};

inline Mat4 Mat4_make_rotate_y(double angle){
	Mat4 result = Mat4_id();
	double cos_a = cos(angle);
	double sin_a = sin(angle);

	result.m[0][0] = cos_a;
	result.m[0][2] = sin_a;
	result.m[2][0] = -sin_a;
	result.m[2][2] = cos_a;

	return result;
};

Mat4 Mat4_mult_mat4(Mat4 a, Mat4 b){
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
