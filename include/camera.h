#pragma once

#include "vec.h"
#include "mat.h"

struct Input;

struct Camera{
	Camera(Vec3 _position);
	Mat4 view_matrix() const;
	void update(const Input& input);

	Vec3 position;
	Vec3 front;
	float speed;
	private:
		float m_yaw;
		float m_pitch;
		float m_prevx;
		float m_prevy;
		bool m_first_update;
};

