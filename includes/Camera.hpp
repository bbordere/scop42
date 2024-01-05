#pragma once

#include "Utils.hpp"
#include "Vectors.hpp"

class Camera {
	public:
		vec3f pos;
		vec3f target;
		vec3f up;
		vec3f rotation;

		Camera();
		void reset();
		void rotationHandling();
};
