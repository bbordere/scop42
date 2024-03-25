#pragma once

#include "Color.hpp"
#include "Vectors.hpp"

// clang-format off

struct Face {
	vec3f vertices[3];
	vec2f texCoords[3];
	vec3f normals[3];
	ColorF color;

	Face();
};

// clang-format on