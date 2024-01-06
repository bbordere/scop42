#include "Camera.hpp"

Camera::Camera() {
	this->reset();
}

void Camera::reset() {
	this->pos = vec3f(-5, 0, 0);
	this->target = vec3f(0, 0, -1);
	this->up = vec3f(0, 1, 0);
	this->rotation = vec3f(0, 0, 0);
}

void Camera::rotationHandling() {
	this->target.x = std::cos(degToRad(this->rotation.y)) *
					 std::cos(degToRad(this->rotation.x));

	this->target.y = std::sin(degToRad(this->rotation.x));

	this->target.z = std::sin(degToRad(this->rotation.y)) *
					 std::cos(degToRad(this->rotation.x));
	this->target.normalize();

	this->up = vec3f::normalize(vec3f::cross(
		vec3f::normalize(vec3f::cross(this->target, vec3f(0, 1, 0))),
		this->target));
}
