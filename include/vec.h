#pragma once
typedef struct{
	double x, y, z;
}Vec3f;

typedef struct{
	double x, y;
}Vec2f;

Vec3f Vec3f_rotate_z(const Vec3f v, double angle);
Vec3f Vec3f_rotate_y(const Vec3f v, double angle);
Vec3f Vec3f_rotate_x(const Vec3f v, double angle);
