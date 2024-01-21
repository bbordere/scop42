#pragma once

#include "Utils.hpp"
#include "Vectors.hpp"

class Camera {
	public:
		vec3f pos;
		vec3f target;
		vec3f up;
		vec3f rotation;
		vec3f startingPos;

		Camera();
		void reset();
		void rotationHandling();
		void setStartingPos(vec3f const &startPos);
};
