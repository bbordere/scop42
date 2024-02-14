#pragma once

#include "BmpImage.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Shader.hpp"

class SkyBox {
	private:
		Shader shader;
		GLuint vao = 0;
		GLuint vbo = 0;
		GLuint ebo = 0;
		std::size_t curTex = 0;

		GLuint textures[4];

		float const vertices[24] = {-1.0f, -1.0f, 1.0f,	 1.0f,	-1.0f, 1.0f,
									1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
									-1.0f, 1.0f,  1.0f,	 1.0f,	1.0f,  1.0f,
									1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f};

		unsigned int const indices[36] = {1, 2, 6, 6, 5, 1, 0, 4, 7, 7, 3, 0,
										  4, 5, 6, 6, 7, 4, 0, 3, 2, 2, 1, 0,
										  0, 1, 5, 5, 4, 0, 3, 7, 6, 6, 2, 3};

		Light lightSources[4];

	private:
		void loadSkyboxes(std::string const &dir, std::size_t texId);
		void initLights();

	public:
		SkyBox();
		~SkyBox();

		void init();
		void draw(Camera const &camera, vec2u const &size, GLenum polygonMode);
		void setCurTex(std::size_t val);
		std::size_t getCurTex() const;

		Light const &getLight() const;
};
