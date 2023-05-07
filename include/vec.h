#pragma once
typedef struct{
	double x, y, z;
}Vec3f;

typedef struct{
	double x, y;
}Vec2f;

// vec2 operations
double Vec2f_len(const Vec2f v);
Vec2f Vec2f_add(Vec2f a, Vec2f b);
Vec2f Vec2f_sub(Vec2f a, Vec2f b);
Vec2f Vec2f_mul(Vec2f a, double t);
double Vec2f_dot(Vec2f a, Vec2f b);
Vec2f Vec2f_normalize(Vec2f v);

// vec3 operations
double Vec3f_len(const Vec3f v);
Vec3f Vec3f_add(Vec3f a, Vec3f b);
Vec3f Vec3f_sub(Vec3f a, Vec3f b);
Vec3f Vec3f_mul(Vec3f a, double t);
Vec3f Vec3f_cross(Vec3f a, Vec3f b);
double Vec3f_dot(Vec3f a, Vec3f b);
Vec3f Vec3f_rotate_z(const Vec3f v, double angle);
Vec3f Vec3f_rotate_y(const Vec3f v, double angle);
Vec3f Vec3f_rotate_x(const Vec3f v, double angle);
Vec3f Vec3f_normalize(Vec3f v);
