#pragma once

#include "Camera.hpp"
#include "Shader.hpp"
#include "stb_image.h"

class SkyBox {
	private:
	public:
		Shader shader;
		GLuint vao, vbo, ebo, textureId;

		float const vertices[24] = {-1.0f, -1.0f, 1.0f,	 1.0f,	-1.0f, 1.0f,
									1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
									-1.0f, 1.0f,  1.0f,	 1.0f,	1.0f,  1.0f,
									1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f};

		unsigned int const indices[36] = {1, 2, 6, 6, 5, 1, 0, 4, 7, 7, 3, 0,
										  4, 5, 6, 6, 7, 4, 0, 3, 2, 2, 1, 0,
										  0, 1, 5, 5, 4, 0, 3, 7, 6, 6, 2, 3};

	public:
		SkyBox();
		void init();
		void draw(Camera const &camera, bool isRendered, uint width,
				  uint height, GLenum polygonMode);
};
