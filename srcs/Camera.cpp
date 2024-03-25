#include "Camera.hpp"

Camera::Camera(): startingPos({0.0f, 0.0f, 0.0f}) {
	this->reset();
}

void Camera::reset() {
	this->pos = startingPos;
	this->target = vec3f(0, 0, 0);
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

void Camera::setStartingPos(vec3f const &startPos,
							std::pair<vec3f, vec3f> const &bounds) {
	vec3f min(bounds.first);
	vec3f max(bounds.second);
	this->startingPos = startPos;
	float d = (bounds.second.y - bounds.first.y) / std::tan(45);
	this->startingPos.x -= (d * 4);
}

vec3f const &Camera::getPosition() const {
	return (this->pos);
}

vec3f const &Camera::getRotation() const {
	return (this->rotation);
}

vec3f const &Camera::getTarget() const {
	return (this->target);
}

vec3f const &Camera::getUp() const {
	return (this->up);
}

void Camera::setRotation(vec3f const &newRotation) {
	this->rotation = newRotation;
}

void Camera::rotate(vec3f const &axis) {
	this->rotation += axis;
}

void Camera::movePos(vec3f const &offset) {
	this->pos += offset;
}
