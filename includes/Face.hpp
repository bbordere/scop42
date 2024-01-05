#pragma once

#include "Utils.hpp"
#include "Vectors.hpp"

class Face {
	public:
		vec3f vertices[3];
		vec2f texCoords[3];
		vec3f normal;
		t_color color;

	public:
		Face();
};
