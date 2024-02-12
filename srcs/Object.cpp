#include "Object.hpp"

void Object::configFromFile(File3D const &file) {
	this->vertices.clear();
	this->scaleFactors = {1, 1, 1};
	this->model = mat4f::makeIdentity();
	this->computeModelInfos(file);
	std::vector<Face> const &faces = file.getFaces();
	for (Face const &f : faces) {
		for (int i = 0; i < 3; ++i) {
			vertices.emplace_back(
				Vertex(f.vertices[i], f.color, f.texCoords[i], f.normals[i]));
		}
	}
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
				 this->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(Vertex)),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (sizeof(Vertex)),
						  (void *)(offsetof(Vertex, color)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (sizeof(Vertex)),
						  (void *)(offsetof(Vertex, texPos)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (sizeof(Vertex)),
						  (void *)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Object::draw(Shader &shader) const {
	shader.use();
	shader.setUniform("model", this->model);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

void Object::rotate(float angle, vec3f const &axis) {
	this->model.rotate(angle, axis);
}

vec3f const &Object::getCenter() const {
	return (this->center);
}

vec3f const &Object::getMinCoord() const {
	return (this->boundMin);
}

void Object::translate(vec3f const &axis) {
	this->model.translate(axis);
	this->center += axis;
}

void Object::scale(vec3f const &scale) {
	this->model.scale(scale);
	this->scaleFactors = scale;

	this->center.x /= (1 / scale.x);
	this->center.y /= (1 / scale.y);
	this->center.z /= (1 / scale.z);

	this->boundMin.x /= (1 / scale.x);
	this->boundMin.y /= (1 / scale.y);
	this->boundMin.z /= (1 / scale.z);

	this->boundMax.x /= (1 / scale.x);
	this->boundMax.y /= (1 / scale.y);
	this->boundMax.z /= (1 / scale.z);
}

mat4f const &Object::getModel() const {
	return (this->model);
}

vec3f const &Object::getScalingFactors() const {
	return (this->scaleFactors);
}

void Object::computeModelInfos(File3D const &file) {
	std::vector<Face> const &faces = file.getFaces();

	this->boundMax = faces[0].vertices[0];
	this->boundMin = faces[0].vertices[0];
	for (Face const &face : faces) {
		for (int i = 0; i < 3; ++i) {

			this->boundMax.x = std::max(this->boundMax.x, face.vertices[i].x);
			this->boundMax.y = std::max(this->boundMax.y, face.vertices[i].y);
			this->boundMax.z = std::max(this->boundMax.z, face.vertices[i].z);

			this->boundMin.x = std::min(this->boundMin.x, face.vertices[i].x);
			this->boundMin.y = std::min(this->boundMin.y, face.vertices[i].y);
			this->boundMin.z = std::min(this->boundMin.z, face.vertices[i].z);
		}
	}

	this->center.x = (this->boundMax.x + this->boundMin.x) / 2;
	this->center.y = (this->boundMax.y + this->boundMin.y) / 2;
	this->center.z = (this->boundMax.z + this->boundMin.z) / 2;
	float factor = 1 / ((this->boundMax.y - this->boundMin.y) / 4);
	if (factor < 1.0 || factor > 10.0)
		this->scale(vec3f(factor, factor, factor));
}

std::pair<vec3f, vec3f> Object::getBounds() const {
	return (std::make_pair(this->boundMin, this->boundMax));
}

Object::~Object() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	// 8600 / 180 453 / 1 771 / 460 583
}
