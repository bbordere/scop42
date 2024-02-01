#include "Shader.hpp"

Shader::Shader() {}

GLint Shader::loadShaderFile(std::string const &path, GLenum type) {
	std::string content;
	std::ifstream file;
	std::stringstream stream;
	char infoLog[512];
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	file.open(path);
	stream << file.rdbuf();
	file.close();
	content = stream.str();

	GLuint shader = glCreateShader(type);
	char const *constContent = content.c_str();
	glShaderSource(shader, 1, &constContent, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		throw std::runtime_error(infoLog);
	};
	glAttachShader(this->programId, shader);
	return (shader);
}

Shader::Shader(std::string const &fragPath, std::string const &vertPath,
			   std::string const &geomPath) {
	this->programId = glCreateProgram();
	char infoLog[512];
	GLint success;
	GLint frag = loadShaderFile(fragPath, GL_FRAGMENT_SHADER);
	GLint vert = loadShaderFile(vertPath, GL_VERTEX_SHADER);
	GLint geom = -1;
	if (!geomPath.empty())
		geom = loadShaderFile(geomPath, GL_GEOMETRY_SHADER);
	glLinkProgram(this->programId);
	glDeleteShader(frag);
	glDeleteShader(vert);
	if (geom != -1)
		glDeleteShader(geom);
	glGetProgramiv(this->programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->programId, 512, NULL, infoLog);
		throw std::runtime_error(std::string(infoLog));
	}
}

void Shader::use() const {
	glUseProgram(this->programId);
}

void Shader::setUniformImpl(std::string const &name, bool value) const {
	glUniform1i(glGetUniformLocation(this->programId, name.c_str()), value);
}

void Shader::setUniformImpl(std::string const &name, int value) const {
	glUniform1i(glGetUniformLocation(this->programId, name.c_str()), value);
}

void Shader::setUniformImpl(std::string const &name, float value) const {
	glUniform1f(glGetUniformLocation(this->programId, name.c_str()), value);
}

void Shader::setUniformImpl(std::string const &name, vec3f const &value) const {
	glUniform3fv(glGetUniformLocation(this->programId, name.c_str()), 1,
				 &value.x);
}

void Shader::setUniformImpl(std::string const &name, mat4f const &value) const {
	glUniformMatrix4fv(glGetUniformLocation(this->programId, name.c_str()), 1,
					   GL_FALSE, value.getDataPtr());
}
