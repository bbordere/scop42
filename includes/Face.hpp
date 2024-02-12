#pragma once

#include "Color.hpp"
#include "Vectors.hpp"

class Face {
	public:
		vec3f vertices[3];
		vec2f texCoords[3];
		vec3f normals[3];
		ColorF color;

	public:
		Face();
};
