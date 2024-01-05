#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

class Shader {
	private:
	public:
	unsigned int programId;
	Shader();
	Shader(std::string const &fragPath, std::string const &vertPath);

	void use(void) const;

	template <typename T>
	void setUniform(std::string const &name, T value) const {
		setUniformImpl(name, value);
	}

	private:
	GLint loadShaderFile(std::string const &path, GLenum type);

	void setUniformImpl(std::string const &name, bool value) const;
	void setUniformImpl(std::string const &name, int value) const;
	void setUniformImpl(std::string const &name, float value) const;
};
