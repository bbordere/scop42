#pragma once

#include "File3D.hpp"
#include "Mat4.hpp"
#include "Shader.hpp"
#include "Vectors.hpp"
#include "glad/glad.h"
#include <vector>

class Object {
	private:
		GLuint vbo;
		GLuint vao;
		std::vector<float> vertices;
		mat4f model;
		vec3f center;
		vec3f minCoord;

	public:
		void configFromFile(File3D const &file);
		void draw(Shader const &shader) const;
		void rotate(float angle, vec3f const &axis, vec3f const &center);
		vec3f const &getCenter() const;
		vec3f const &getMinCoord() const;
		void translate(vec3f const &axis);
		void scale(vec3f const &scale);
};
