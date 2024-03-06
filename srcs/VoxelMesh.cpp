#include "VoxelMesh.hpp"

void VoxelMesh::init(Object const &obj) {
	for (Vertex const &v : obj.vertices) {
		this->vertices.push_back(v.pos * VOXEL_SIZE);
	}

	this->shader = Shader("shaders/voxel.frag", "shaders/voxel.vert",
						  "shaders/voxel.geom");

	std::cout << this->vertices << '\n';

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(vec3f),
				 this->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
						  (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VoxelMesh::draw(mat4f const &proj, mat4f const &view, mat4f const &model) {
	this->shader.use();
	this->shader.setUniform("projection", proj);
	this->shader.setUniform("view", view);
	this->shader.setUniform("model", model);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_POINTS, 0, this->vertices.size());
	glBindVertexArray(0);
}
