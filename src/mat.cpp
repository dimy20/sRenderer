#include "mat.h"

Mat4 look_at(const Vec3& eye, const Vec3& target, const Vec3& up){
	Vec3 Rz = normalize(target - eye);
	Vec3 Rx = normalize(cross(up, Rz));
	Vec3 Ry = cross(Rz, Rx);

	Mat4 result = {{
		{Rx.x, Rx.y, Rx.z, -dot(Rx, eye)},
		{Ry.x, Ry.y, Ry.z, -dot(Ry, eye)},
		{Rz.x, Rz.y, Rz.z, -dot(Rz, eye)},
		{   0,    0,    0,             1}
	}};

	return result;
}
