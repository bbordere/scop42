#include "SkyBox.hpp"

SkyBox::SkyBox() {}

void SkyBox::init() {
	this->shader =
		Shader("shaders/skyboxFragment.glsl", "shaders/skyboxVertex.glsl");
	this->shader.use();
	this->shader.setUniform("skybox", 0);
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), &this->vertices,
				 GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), &this->indices,
				 GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	std::string facesCubemap[6] = {"skybox/right.jpg", "skybox/left.jpg",
								   "skybox/top.jpg",   "skybox/bottom.jpg",
								   "skybox/front.jpg", "skybox/back.jpg"};
	glGenTextures(1, &this->textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < 6; i++) {
		int width, height, nrChannels;
		unsigned char *data =
			stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
						 height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture: " << facesCubemap[i]
					  << std::endl;
			stbi_image_free(data);
		}
	}
}

void SkyBox::draw(Camera const &camera, bool isRendered, uint width,
				  uint height) {
	if (!isRendered)
		return;
	glDepthFunc(GL_LEQUAL);
	this->shader.use();
	mat4f view = mat4f::makeIdentity();
	mat4f projection = mat4f::makeIdentity();
	view = mat4f::lookAt(camera.pos, camera.pos + camera.target, camera.up);
	view.transformMat3();
	projection =
		mat4f::makePerspective(45, (float)width / height, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(this->shader.programId, "view"), 1,
					   GL_FALSE, view.getDataPtr());
	glUniformMatrix4fv(
		glGetUniformLocation(this->shader.programId, "projection"), 1, GL_FALSE,
		projection.getDataPtr());

	glBindVertexArray(this->vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
