#pragma once
#include "Vectors.hpp"

class Light {
	private:
	public:
		vec3f pos;
		vec3f color;

		vec3f ambientIntensity;
		vec3f diffuseIntensity;
		vec3f specularIntensity;

	public:
		Light(vec3f pos, vec3f color);
};
