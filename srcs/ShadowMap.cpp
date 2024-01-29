#include "ShadowMap.hpp"

void ShadowMap::init() {
	this->shader = Shader("shaders/shadowMap.frag", "shaders/shadowMap.vert");
	this->texture.createBlank(SHADOW_RES, SHADOW_RES);
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
						   this->texture.getId(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Shader const &ShadowMap::getShader() const {
	return (this->shader);
}

GLuint ShadowMap::getFbo() const {
	return (this->fbo);
}

GLuint ShadowMap::getTexture() const {
	return (this->texture.getId());
}

void ShadowMap::bindTexture() const {
	this->texture.bind();
}
