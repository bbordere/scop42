#pragma once

#include "Color.hpp"
#include "Vectors.hpp"

class Vertex {
	public:
		vec3f pos;
		ColorF color;
		vec2f texPos;
		vec3f normal;

		Vertex(vec3f pos, ColorF color, vec2f texPos, vec3f normal)
			: pos(pos), color(color), texPos(texPos), normal(normal){};
};
