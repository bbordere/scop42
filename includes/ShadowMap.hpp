#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#define SHADOW_RES 8192

class ShadowMap {
	private:
		GLuint fbo;
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
