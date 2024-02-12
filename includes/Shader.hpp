#pragma once
#include "Mat4.hpp"
#include "Vectors.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <map>
#include <sstream>

class Shader {

	private:
		unsigned int programId;
		std::map<std::string, GLint> locations;

	public:
		Shader();
		Shader(std::string const &fragPath, std::string const &vertPath,
			   std::string const &geomPath = "");
		~Shader();

		void use(void) const;

		GLuint getId() const;

		template <typename T>
		void setUniform(std::string const &name, T value) {
			if (!locations.count(name))
				locations[name] =
					glGetUniformLocation(this->programId, name.c_str());
			setUniformImpl(locations[name], value);
		}

	private:
		GLint loadShaderFile(std::string const &path, GLenum type);

		void setUniformImpl(GLint location, bool value) const;
		void setUniformImpl(GLint location, int value) const;
		void setUniformImpl(GLint location, float value) const;
		void setUniformImpl(GLint location, vec3f const &value) const;
		void setUniformImpl(GLint location, mat4f const &value) const;
};
