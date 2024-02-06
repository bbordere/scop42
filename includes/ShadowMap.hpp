#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#define SHADOW_RES 4096

class ShadowMap {
	private:
		GLuint fbo;
		Texture texture;
		Shader shader;

	public:
		void init();
		Shader const &getShader() const;
		GLuint getFbo() const;
		GLuint getTexture() const;
		void bindTexture() const;
};
