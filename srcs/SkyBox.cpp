#include "SkyBox.hpp"

SkyBox::SkyBox() {}

void SkyBox::loadSkyboxes(std::string const &dir, std::size_t texId) {
	std::string paths[6] = {dir + "right.bmp", dir + "left.bmp",
							dir + "top.bmp",   dir + "bottom.bmp",
							dir + "front.bmp", dir + "back.bmp"};
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textures[texId]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	BmpImage image;
	for (unsigned int i = 0; i < 6; i++) {
		try {
			image.extractData(paths[i]);
			image.reverse();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
						 image.getInfoHeader().imgWidth,
						 image.getInfoHeader().imgHeight, 0, GL_RGBA,
						 GL_UNSIGNED_BYTE, image.getData());
		}
		catch (std::exception const &e) {
			std::cerr << e.what() << '\n';
		}
	}
}

void SkyBox::initLights() {

	// clang-format off
	vec3f pos[] = {
		{-5, 6,	6},
		{9,	10,	4},
		{-12, 2.4, 1.7},
		{-6.8, 7.2, -11},
	};

	vec3f colors[] = {
		{0.98, 0.92, 0.96},
		{1,	1, 0.9 },
		{0.75,	0.75, 0.75},
		{1,	0.70, 0.4},
	};
	// clang-format on

	for (int i = 0; i < 4; ++i) {
		lightSources[i] = Light(pos[i], colors[i]);
		lightSources[i].initMatrixes();
	}
	lightSources[2].setIntensities({0.75, 0.8, 1});
}

Light const &SkyBox::getLight() const {
	return (this->lightSources[this->curTex]);
}

void SkyBox::init() {
	this->shader = Shader("shaders/skybox.frag", "shaders/skybox.vert");
	this->shader.use();
	this->shader.setUniform("skybox", 0);
	this->initLights();

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

	glGenTextures(4, this->textures);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textures[0]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	this->loadSkyboxes("skybox/default/", 0);
	this->loadSkyboxes("skybox/beach/", 1);
	this->loadSkyboxes("skybox/ice/", 2);
	this->loadSkyboxes("skybox/sand/", 3);
}

void SkyBox::draw(Camera const &camera, vec2u const &size, GLenum polygonMode) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthFunc(GL_LEQUAL);
	this->shader.use();
	mat4f view = mat4f::makeIdentity();
	mat4f projection = mat4f::makeIdentity();
	view = mat4f::lookAt(camera.pos, camera.pos + camera.target, camera.up);
	view.transformMat3();
	projection =
		mat4f::makePerspective(45, (float)size.x / size.y, 0.1f, 20.0f);

	this->shader.setUniform("view", view);
	this->shader.setUniform("projection", projection);

	glBindVertexArray(this->vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textures[this->curTex]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
}

void SkyBox::setCurTex(std::size_t val) {
	this->curTex = val;
}

std::size_t SkyBox::getCurTex() const {
	return (this->curTex);
}

SkyBox::~SkyBox() {
	if (this->vao) {
		glDeleteTextures(4, this->textures);
		glDeleteBuffers(1, &this->vbo);
		glDeleteBuffers(1, &this->ebo);
		glDeleteVertexArrays(1, &this->vao);
	}
}
