#pragma once

#include "Utils.hpp"
#include "Vectors.hpp"

class Camera {
	private:
		vec3f pos;
		vec3f target;
		vec3f up;
		vec3f rotation;
		vec3f startingPos;

	public:
		Camera();
		void reset();
		void rotationHandling();
		void setStartingPos(vec3f const &startPos,
							std::pair<vec3f, vec3f> const &bounds);
		vec3f const &getRotation() const;
		vec3f const &getPosition() const;
		vec3f const &getTarget() const;
		vec3f const &getUp() const;
		void setRotation(vec3f const &newRotation);
		void rotate(vec3f const &axis);
		void movePos(vec3f const &offset);
};
