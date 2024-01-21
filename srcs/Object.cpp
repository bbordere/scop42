#include "Object.hpp"

void Object::configFromFile(File3D const &file) {
	this->center = file.getCenter();
	this->model = mat4f::makeIdentity();
	std::vector<Face> const &faces = file.getFaces();
	for (Face f : faces) {
		for (int i = 0; i < 3; ++i) {
			vertices.push_back(f.vertices[i].x);
			vertices.push_back(f.vertices[i].y);
			vertices.push_back(f.vertices[i].z);

			vertices.push_back(f.color.r);
			vertices.push_back(f.color.g);
			vertices.push_back(f.color.b);

			vertices.push_back(f.texCoords[i].x);
			vertices.push_back(f.texCoords[i].y);

			vertices.push_back(f.normals[i].x);
			vertices.push_back(f.normals[i].y);
			vertices.push_back(f.normals[i].z);
		}
	}
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float),
				 this->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (11 * sizeof(float)),
						  (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)),
						  (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Object::draw(Shader const &shader) const {
	shader.setUniform("model", this->model);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	glBindVertexArray(0);
}

void Object::rotate(float angle, vec3f const &axis, vec3f const &center) {
	this->model.translate(center * -1);
	this->model.rotate(angle, axis);
	this->model.translate(center);
}

vec3f const &Object::getCenter() const {
	return (this->center);
}

void Object::translate(vec3f const &axis) {
	this->model.translate(axis);
}
