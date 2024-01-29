#pragma once

#include "Face.hpp"
#include "Utils.hpp"
#include "Vectors.hpp"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <limits>
#include <regex>
#include <vector>

class File3D {
	private:
		std::vector<Face> faces;
		std::vector<vec3f> vertices;
		std::vector<vec3f> normals;
		std::vector<vec2f> texCoords;
		vec3f center;
		vec3f boundVector;
		vec3f minCoord;

		t_color const colorsPalette[4] = {
			{0.3f, 0.3f, 0.3f},
			{0.6f, 0.6f, 0.6f},
			{0.2f, 0.2f, 0.2f},
			{0.4f, 0.4f, 0.4f},
		};

	public:
		void load(std::string const &path);
		std::vector<Face> const &getFaces() const;
		vec3f const &getCenter() const;
		vec3f const &getBoundVec() const;
		vec3f const &getMinCoord() const;

	private:
		void parseFace(std::vector<std::string> const &input);

		vec3f const &getVertice(std::size_t i) const;
		vec3f const &getNormal(std::size_t i) const;
		vec2f const &getTexCoord(std::size_t i) const;

		Face vertexOnly(std::vector<std::string> const &input, std::size_t v1,
						std::size_t v2, std::size_t v3);

		Face vertexTexture(std::vector<std::string> const &input,
						   std::size_t v1, std::size_t v2, std::size_t v3);

		Face vertexTextureNormal(std::vector<std::string> const &input,
								 std::size_t v1, std::size_t v2,
								 std::size_t v3);

		Face vertexNormal(std::vector<std::string> const &input, std::size_t v1,
						  std::size_t v2, std::size_t v3);

		Face makeFace(std::vector<std::string> const &input, std::size_t v1,
					  std::size_t v2, std::size_t v3);

		void computeCenter();
		void computeNormals(Face &face);
		void computeUV(Face &face);
};
