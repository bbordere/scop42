#include "Face.hpp"

Face::Face() {
	for (int i = 0; i < 3; ++i) {
		this->vertices[i] = {-1.0f, -1.0f, -1.0f};
		this->texCoords[i] = {-1.0f, -1.0f};
		this->normals[i] = {-1.0f, -1.0f, -1.0f};
	}
	this->color = {0, 0, 0, 1};
}
