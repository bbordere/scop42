#pragma once

#include "Mat4.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include <vector>

constexpr float VOXEL_SIZE = 1;

class VoxelMesh {
	private:
		GLuint vao;
		GLuint vbo;
		std::vector<vec3f> vertices;
		Shader shader;

	public:
		void init(Object const &obj);
		void draw(mat4f const &proj, mat4f const &view, mat4f const &model);
};
