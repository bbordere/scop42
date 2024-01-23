#pragma once

#include "Shader.hpp"
#define SHADOW_RES 2048

class ShadowMap {
	private:
		GLuint fbo, textureId;
		Shader shader;

	public:
		void init();
		Shader const &getShader() const;
		GLuint getFbo() const;
		GLuint getTexture() const;
};
