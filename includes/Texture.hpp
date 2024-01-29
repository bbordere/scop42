#pragma once

#include "glad/glad.h"
#include <iostream>

class Texture {
	public:
		void loadFromFile(std::string const &path);
		void createBlank(int width, int height);
		void setParameter(GLenum param, GLint value);
		void bind() const;

		GLuint getId() const;

	private:
		GLuint id;
};
