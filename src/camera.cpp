#include <iostream>
#include <cmath>

#include "camera.h"
#include "mat.h"
#include "input.h"

static const Vec3 UP(0, 1.0, 0);
#define DEFAULT_CAMERA_SPEED 0.1

Camera::Camera(Vec3 _position) 
: position(_position), front(0, 0, 1.0) {
	/* This values make sure that the first time update is called
	 * we get the (0, 0, 1.0) vector.*/
	m_pitch = 0.0; 
	m_yaw = 90.0f;
	speed = DEFAULT_CAMERA_SPEED;
	m_first_update = true;
};

Mat4 Camera::view_matrix() const {
	return look_at(position, position + front, UP);
};


void Camera::update(const Input& input){
	// camera translation
	if(input.keyboard[SDL_SCANCODE_W]){
		position += speed * front;
	}
	if(input.keyboard[SDL_SCANCODE_S]){
		position -= speed * front;
	}
	if(input.keyboard[SDL_SCANCODE_D]){
		position += normalize(cross(UP, front)) * speed;
	}
	if(input.keyboard[SDL_SCANCODE_A]){
		position -= normalize(cross(UP, front)) * speed;
	}

	// camera rotation
	float sens = 0.2f;
	float offset_x, offset_y;

	if(m_first_update){
		m_prevx = input.mouse_x;
		m_prevy = input.mouse_y;
		m_first_update = false;
	}

	offset_x = (input.mouse_x - m_prevx) * sens;
	offset_y = (input.mouse_y - m_prevy) * sens;

	m_prevx = input.mouse_x;
	m_prevy = input.mouse_y;

	// update yaw and pitch based on the offsets from the previous x and y positions;
	m_yaw += offset_x;
	m_pitch += offset_y;

	if(m_pitch > 89.0f) m_pitch = 89.0f;
	if(m_pitch < -89.0f) m_pitch = -89.0f;

	// update direction vector based on new yaw and pitch values.
	Vec3 new_front;
	new_front.x = -std::cos(to_rad(m_yaw)) * std::cos(to_rad(m_pitch));
	new_front.y = -std::sin(to_rad(m_pitch));
	new_front.z = std::sin(to_rad(m_yaw)) * std::cos(to_rad(m_pitch));

	front = normalize(new_front);
};
