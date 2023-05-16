#pragma once

#include <cmath>
#include <cstdio>
#include <ostream>
struct Vec4{
	Vec4() : x(0), y(0), z(0), w(0) {};
	Vec4(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), w(_w) {};
	union{
		struct{
			double x, y, z, w;
		};
		double values[4];
	};
};

template<typename T>
struct Vec2{
	Vec2() : x(0), y(0) {};
	Vec2(T _x, T _y) : x(_x), y(_y) {};
	constexpr double length() const { return sqrt(x*x + y*y); }
	template<typename U>
	friend std::ostream& operator << (std::ostream& out, const Vec2<U>& vec);
	T x, y;
};

template<typename U>
inline std::ostream& operator << (std::ostream& out, const Vec2<U>& vec){
	out << "(" << vec.x << ", " << vec.y << ")";
	return out;
}

template<typename T>
inline Vec2<T> operator+(const Vec2<T>& a, const Vec2<T>& b){
	return Vec2<T>(a.x + b.x, a.y + b.y);
};

template<typename T>
inline Vec2<T> operator-(const Vec2<T>& a, const Vec2<T>& b){
	return Vec2<T>(a.x - b.x, a.y - b.y);
};

template<typename T>
inline Vec2<T> operator*(const Vec2<T>& a, double t){
	return Vec2<T>(a.x * t, a.y * t);
};

template<typename T>
inline double dot(const Vec2<T>& a, const Vec2<T>& b){
	return a.x*b.x + a.y*b.y;
};

template<typename T>
inline Vec2<T> normalize(const Vec2<T>& a){
	double len = a.length();
	return Vec2<T>(a.x /len, a.y / len);
}

typedef Vec2<int> Vec2i;
typedef Vec2<double> Vec2f;

struct Vec3{
	Vec3() : x(0), y(0), z(0) {};
	Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
	inline double length() const { return sqrt(x*x + y*y + z*z); };
	inline Vec4 augmented() const { return Vec4(x, y, z, 1.0); };
	friend std::ostream& operator << (std::ostream& out, const Vec3& vec);
	inline Vec3& operator += (const Vec3& other){
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	};

	double x, y, z;
};

inline std::ostream& operator << (std::ostream& out, const Vec3& vec){
	char log[128] = {0};
	snprintf(log, 128, "(%f, %f, %f)", vec.x, vec.y, vec.z);
	out << log;
	return out;
};

inline Vec3 operator+(const Vec3& a, const Vec3& b){ return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vec3 operator-(const Vec3& a, const Vec3& b){ return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vec3 operator*(const Vec3& a, double t){ return Vec3(a.x * t, a.y * t, a.z * t); }
inline double dot(const Vec3& a, const Vec3&b){ return a.x*b.x + a.y*b.y + a.z*b.z; }

inline Vec3 normalize(const Vec3& a){
	double len = a.length();
	return Vec3(a.x / len, a.y / len, a.z / len);
}

inline Vec3 cross(const Vec3& a, const Vec3& b){
	double x, y, z;
	x = a.y*b.z - a.z*b.y;
	y = a.z*b.x - a.x*b.z;
	z = a.x*b.y - a.y*b.x;
	return Vec3(x, y, z);
};

inline Vec3 Vec3_rotate_z(const Vec3& v, double angle){
	double rotated_x = v.x * cos(angle) - v.y * sin(angle);
	double rotated_y = v.x * sin(angle) + v.y * cos(angle);
	return Vec3(rotated_x, rotated_y, v.z);
};

inline Vec3 Vec3_rotate_x(const Vec3& v, double angle){
	double rotated_y = v.y * cos(angle) - v.z * sin(angle);
	double rotated_z = v.y * sin(angle) + v.z * cos(angle);
	return Vec3(v.x, rotated_y, rotated_z);
};

inline Vec3 Vec3_rotate_y(const Vec3& v, double angle){
	double rotated_x = v.x * cos(angle) + v.z * sin(angle);
	double rotated_z = v.x * -sin(angle) + v.z * cos(angle);
	return Vec3(rotated_x, v.y, rotated_z);
}

inline Vec3 vec3_from_vec4(const Vec4& v) { return Vec3(v.x, v.y, v.z); };
inline Vec4 vec4_from_vec3(const Vec3& v) { return Vec4(v.x, v.y, v.z, 1.0); };

typedef Vec3 Color;
