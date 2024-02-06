#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Texture::loadFromFile(std::string const &path) {
	BmpImage image;
	image.extractData(path);
	void *data = image.data.data();
	int width = image.getInfoHeader().imgWidth;
	int height = image.getInfoHeader().imgHeight;
	// int width, height, nrChannels;
	// void *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	// if (!data)
	// throw std::runtime_error("Can't open texture file !");
	glGenTextures(1, &this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
				 GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);
	// stbi_image_free(data);
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
