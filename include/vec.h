#pragma once

#define VEC2_PRINT(v) do{ \
	printf("(%f, %f)\n", v.x, v.y); \
}while(0);

#define VEC3_PRINT(v) do{ \
	printf("(%f, %f, %f)\n", v.x, v.y, v.z); \
}while(0);

#define VEC4_PRINT(v) do{ \
	printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w); \
}while(0)

typedef struct{
	double x, y;
}Vec2;

typedef struct{
	double x, y, z;
}Vec3;

typedef struct{
	union{
		struct{
			double x, y, z, w;
		};
		double values[4];
	};
}Vec4;

// vec2 operations
double Vec2_len(const Vec2 v);
Vec2 Vec2_add(Vec2 a, Vec2 b);
Vec2 Vec2_sub(Vec2 a, Vec2 b);
Vec2 Vec2_mul(Vec2 a, double t);
double Vec2_dot(Vec2 a, Vec2 b);
Vec2 Vec2_normalize(Vec2 v);

// vec3 operations
double Vec3_len(const Vec3 v);
Vec3 Vec3_add(Vec3 a, Vec3 b);
Vec3 Vec3_sub(Vec3 a, Vec3 b);
Vec3 Vec3_mul(Vec3 a, double t);
Vec3 Vec3_cross(Vec3 a, Vec3 b);
double Vec3_dot(Vec3 a, Vec3 b);
Vec3 Vec3_rotate_z(const Vec3 v, double angle);
Vec3 Vec3_rotate_y(const Vec3 v, double angle);
Vec3 Vec3_rotate_x(const Vec3 v, double angle);
Vec3 Vec3_normalize(Vec3 v);

//Vec4
Vec4 Vec4_from_vec3(Vec3 vec);
Vec3 Vec3_from_vec4(Vec4 vec);
