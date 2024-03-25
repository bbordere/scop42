#pragma once

#include "File3D.hpp"
#include "Mat4.hpp"
#include "Shader.hpp"
#include "Vectors.hpp"
#include "Vertex.hpp"
#include "glad/glad.h"
#include <utility>
#include <vector>

class Object {
	private:
		GLuint vbo = 0;
		GLuint vao = 0;
		std::vector<Vertex> vertices;
		mat4f model;
		vec3f center;
		vec3f boundMin;
		vec3f boundMax;
		vec3f scaleFactors;
		bool isTextured;

	public:
		~Object();
		void configFromFile(File3D const &file);
		void draw(Shader &shader) const;

		void rotate(float angle, vec3f const &axis);
		void scale(vec3f const &scale);
		void translate(vec3f const &axis);

		vec3f const &getCenter() const;
		vec3f const &getMinCoord() const;
		std::pair<vec3f, vec3f> getBounds() const;
		mat4f const &getModel() const;
		vec3f const &getScalingFactors() const;
		bool getTexturedStatus() const;

	private:
		void computeModelInfos(File3D const &file);
};
