#pragma once

#include "Utils.hpp"
#include "Vectors.hpp"

class Face {
	public:
		vec3f vertices[3];
		vec2f texCoords[3];
		vec3f normals[3];
		t_color color;

	public:
		Face();
};
