#include "Light.hpp"

Light::Light(vec3f pos, vec3f color): pos(pos), color(color) {
	this->ambientIntensity = this->color * 0.5f;
	this->diffuseIntensity = this->color * 0.8f;
	this->specularIntensity = this->color * 1.0f;
}
