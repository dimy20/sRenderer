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

Mat4 Mat4_make_perspective(double fov, double aspect_ratio, double znear, double zfar){
	Mat4 result = {{{0}}};

	result.m[0][0] = aspect_ratio * (1.0 / tan(fov / 2.0));
	result.m[1][1] = 1 / tan(fov / 2.0);
	result.m[2][2] = zfar / (zfar - znear);
	result.m[2][3] = (-zfar * znear) / (zfar - znear);
	result.m[3][2] = 1.0; // keep z

	return result;
};

Vec4 Mat4_mult_vec4_project(Mat4 perspective_proj_mat, Vec4 world_vertex){
	Vec4 projected_vertex = Mat4_mult_vec4(perspective_proj_mat, world_vertex);

	/* Perform perspective division:
	   original Z depth, World z value of v is kept in W component of 
	   projected_v, we can use this value to perform perspective division. */
	if(projected_vertex.w != 0.0){
		projected_vertex.x /= projected_vertex.w;
		projected_vertex.y /= projected_vertex.w;
		projected_vertex.z /= projected_vertex.w;
	};

	return projected_vertex;
};
