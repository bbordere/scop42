#include "Light.hpp"

Light::Light() {}

Light::Light(vec3f pos, vec3f color): pos(pos), color(color) {
	this->ambientIntensity = this->color * 0.5;
	this->diffuseIntensity = this->color * 0.8f;
	this->specularIntensity = this->color * 1.0f;
}

void Light::initMatrixes() {
	this->projection = mat4f::makeOrthogonal(-10, 10, -10, 10, 1, 20);
	this->view = mat4f::lookAt(this->pos, {0, 0, 0}, {0, 1, 0});
	this->space = this->projection * this->view;
}

mat4f const &Light::getSpace() const {
	return (this->space);
}

void Light::setIntensities(vec3f const &intensities) {
	this->ambientIntensity = this->color * intensities.x;
	this->diffuseIntensity = this->color * intensities.y;
	this->specularIntensity = this->color * intensities.z;
}
