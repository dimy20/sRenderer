#pragma once

#include "vec.h"

typedef struct{
	double m[4][4];
}Mat4;

Mat4 Mat4_id(void);
Mat4 Mat4_scale(double sx, double sy, double sz);
Mat4 Mat4_translate(double tx, double ty, double tz);
Mat4 Mat4_make_rotate_x(double angle);
Mat4 Mat4_make_rotate_y(double angle);
Mat4 Mat4_make_rotate_z(double angle);

Vec4 Mat4_mult_vec4(Mat4 mat, Vec4 vec);
Mat4 Mat4_mult_mat4(Mat4 a, Mat4 b);



