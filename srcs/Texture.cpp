#include "Texture.hpp"

Texture::~Texture() {
	if (this->id != 0)
		glDeleteTextures(1, &this->id);
}

void Texture::loadFromFile(std::string const &path) {
	BmpImage image;
	image.extractData(path);
	void const *data = image.getData();
	int width = image.getInfoHeader().imgWidth;
	int height = image.getInfoHeader().imgHeight;

	glGenTextures(1, &this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);
	image.clear();
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::setParameter(GLenum param, GLint value) {
	glTexParameteri(GL_TEXTURE_2D, param, value);
}

void Texture::createBlank(int width, int height) {
	float color[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
				 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

GLuint Texture::getId() const {
	return (this->id);
}
