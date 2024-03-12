#pragma once

#include "Shader.hpp"
#include "Constants.hpp"
#include "Texture.hpp"

class ShadowMap {
	private:
		GLuint fbo = 0;
		Texture texture;
		Shader shader;

	public:
		~ShadowMap();

		void init();
		Shader &getShader();
		GLuint getFbo() const;
		GLuint getTexture() const;
		void bindTexture() const;
};
