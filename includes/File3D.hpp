#pragma once

#include "Color.hpp"
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

		ColorF const colorsPalette[4] = {
			{0.3f, 0.3f, 0.3f, 1.0},
			{0.6f, 0.6f, 0.6f, 1.0},
			{0.2f, 0.2f, 0.2f, 1.0},
			{0.4f, 0.4f, 0.4f, 1.0},
		};

	public:
		void load(std::string const &path);
		std::vector<Face> const &getFaces() const;

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
