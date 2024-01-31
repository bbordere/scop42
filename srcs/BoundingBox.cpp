#include "BoundingBox.hpp"

void BoundingBox::create(Object const &obj) {
	this->shader = Shader("shaders/boundBox.frag", "shaders/boundBox.vert");

	vec3f min = obj.getBounds().first;
	vec3f max = obj.getBounds().second;

	min.x *= (1 / obj.getScalingFactors().x);
	min.y *= (1 / obj.getScalingFactors().y);
	min.z *= (1 / obj.getScalingFactors().z);

	max.x *= (1 / obj.getScalingFactors().x);
	max.y *= (1 / obj.getScalingFactors().y);
	max.z *= (1 / obj.getScalingFactors().z);

	GLfloat vertices[] = {
		min.x, min.y, min.z, // Point 0 (xmin, ymin, zmin)
		max.x, min.y, min.z, // Point 1 (xmin, ymin, zmin)
		max.x, max.y, min.z, // Point 4 (xmin, ymin, zmin)
		min.x, max.y, min.z, // Point 2 (xmin, ymin, zmin)
		min.x, min.y, max.z, // Point 3 (xmin, ymin, zmin)
		max.x, min.y, max.z, // Point 5 (xmin, ymin, zmin)
		min.x, max.y, max.z, // Point 6 (xmin, ymin, zmin)
		max.x, max.y, max.z, // Point 7 (xmin, ymin, zmin)
	};

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint indices[] = {0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 5,
						5, 1, 3, 6, 6, 7, 7, 5, 7, 2, 6, 4};

	glGenBuffers(1, &this->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
						  (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void BoundingBox::draw(mat4f const &proj, mat4f const &view,
					   mat4f const &model) {
	this->shader.use();
	this->shader.setUniform("model", model);
	this->shader.setUniform("projection", proj);
	this->shader.setUniform("view", view);
	glBindVertexArray(this->vao);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
