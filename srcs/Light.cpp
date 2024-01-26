#include "Light.hpp"

Light::Light(vec3f pos, vec3f color): pos(pos), color(color) {
	this->ambientIntensity = this->color * 0.5f;
	this->diffuseIntensity = this->color * 0.8f;
	this->specularIntensity = this->color * 1.0f;
}

void Light::initMatrixes() {
	this->projection = mat4f::makeOrthogonal(-10, 10, -10, 10, 0.1, 10);
	this->view = mat4f::lookAt(this->pos, {0, 0, 0}, {0, 1, 0});
	this->space = this->projection * this->view;
}

mat4f const &Light::getSpace() const {
	return (this->space);
}
