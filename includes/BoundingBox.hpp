#pragma once

#include "Mat4.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Vectors.hpp"
#include "glad/glad.h"

class BoundingBox {
	private:
		GLuint vbo, vao, ebo;
		Shader shader;

	public:
		void create(Object const &object);
		void draw(mat4f const &proj, mat4f const &view, mat4f const &model);
};
