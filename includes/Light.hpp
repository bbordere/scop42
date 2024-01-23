#pragma once

#include "Mat4.hpp"
#include "Vectors.hpp"

class Light {
	private:
	public:
		vec3f pos;
		vec3f color;

		vec3f ambientIntensity;
		vec3f diffuseIntensity;
		vec3f specularIntensity;

		mat4f projection;
		mat4f view;
		mat4f space;

	public:
		Light(vec3f pos, vec3f color);
		void initMatrixes();

		mat4f const &getSpace() const;
};
