#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#define SHADOW_RES 4096

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
