#include "App.hpp"

App::App() {}

void App::init(void) {
	if (!glfwInit()) {
		throw std::runtime_error("Cant init OpenGL !");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(800, 600, "scop", NULL, NULL);
	if (!window) {
		throw std::runtime_error("Cant init Window !");
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw std::runtime_error("Cant init GLEW !");
	}
	this->shader = Shader("shaders/fragment.glsl", "shaders/vertex.glsl");
}

void App::inputHandling() {
	static int mode = 0;
	static bool modeSwitching = false;
	static bool rotation = false;
	float cameraSpeed = 4.0f * this->delta;

	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window, true);

	if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS) {
		glfwWaitEventsTimeout(0.7);
		this->isRotating = !this->isRotating;
	}

	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS &&
		!isSpacePressed)
	{
		this->isTextured = !this->isTextured;
		this->isSpacePressed = true;
	}

	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_RELEASE &&
		this->isSpacePressed)
		this->isSpacePressed = false;

	if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS)
		this->isRotating = !this->isRotating;

	if (glfwGetKey(this->window, GLFW_KEY_T) == GLFW_PRESS && !modeSwitching)
		modeSwitching = true;

	if (glfwGetKey(this->window, GLFW_KEY_T) == GLFW_RELEASE && modeSwitching) {
		mode = (++mode) % 3;
		glPolygonMode(GL_FRONT_AND_BACK, this->modes[mode]);
		modeSwitching = false;
	}

	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
		this->camera.pos += (this->camera.target * cameraSpeed);

	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
		this->camera.pos -= (this->camera.target * cameraSpeed);

	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
		this->camera.pos += vec3f::normalize(vec3f::cross(this->camera.target,
														  this->camera.up)) *
							cameraSpeed;
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
		this->camera.pos -= vec3f::normalize(vec3f::cross(this->camera.target,
														  this->camera.up)) *
							cameraSpeed;
	if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		this->camera.pos += (this->camera.up * cameraSpeed);

	if (glfwGetKey(this->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		this->camera.pos -= (this->camera.up * cameraSpeed);

	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
		this->camera.rotation.y -= 0.1f;
	if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		this->camera.rotation.y += 0.1f;
	if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
		this->camera.rotation.x =
			std::min(this->camera.rotation.x + 0.1f, 89.0f);
	if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
		this->camera.rotation.x =
			std::max(this->camera.rotation.x - 0.1f, -89.0f);
	if (glfwGetKey(this->window, GLFW_KEY_F) == GLFW_PRESS)
		this->camera.reset();
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void App::run() {

	Model model3d;
	model3d.load("models/20430_Cat_v1_NEW.obj");

	auto faces = model3d.getFaces();

	Light light({0, 0, -3}, {1, 1, 1});

	std::vector<float> vertices;
	vertices.reserve(faces.size() * 3 * 11);
	for (auto f : faces) {
		for (int i = 0; i < 3; ++i) {
			vertices.push_back(f.vertices[i].x);
			vertices.push_back(f.vertices[i].y);
			vertices.push_back(f.vertices[i].z);

			vertices.push_back(f.color.r);
			vertices.push_back(f.color.g);
			vertices.push_back(f.color.b);

			vertices.push_back(f.texCoords[i].x);
			vertices.push_back(f.texCoords[i].y);

			vertices.push_back(f.normal.x);
			vertices.push_back(f.normal.y);
			vertices.push_back(f.normal.z);
		}
	}

	unsigned int indices[vertices.size() / 11];
	for (int i = 0; i < vertices.size() / 11; ++i) {
		indices[i] = 0;
	}
	// Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind VAO and VBO
	glBindVertexArray(VAO);

	// Copy vertex data to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
				 vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
				 GL_STATIC_DRAW);

	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
	// GL_STATIC_DRAW);

	// Set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (11 * sizeof(float)),
						  (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)),
						  (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	float factor = 0.0f;

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char *data =
		stbi_load("res/20430_cat_diff_v1.jpg", &width, &height, &nrChannels, 0);

	// BmpImage image;
	// image.extractData("res/texture.bmp");
	// uint8_t *data = image.data.data();
	// width = image.getInfoHeader().imgWidth;
	// height = image.getInfoHeader().imgHeight;

	GLuint texture;
	glGenTextures(1, &texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);

	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	mat4f model = mat4f::makeIdentity();
	float lastFrame = 0.0f;
	model.rotate(-90, vec3f(1.0f, 0.0f, 0.0f));
	while (!glfwWindowShouldClose(this->window)) {
		float currentFrame = glfwGetTime();
		this->delta = currentFrame - lastFrame;
		lastFrame = currentFrame;

		this->inputHandling();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->shader.use();

		if (this->isRotating)
			model.rotate(2 * this->delta, vec3f(0.0f, 1.0f, 0.0f));
		// this->modelRotationAngle += 0.05f;

		this->camera.rotationHandling();

		mat4f projection =
			mat4f::makePerspective(45, (float)800 / (float)600, 0.1f, 100.0f);
		mat4f view = mat4f::lookAt(this->camera.pos,
								   this->camera.pos + this->camera.target,
								   this->camera.up);

		unsigned int modelLoc = glGetUniformLocation(shader.programId, "model");
		unsigned int viewLoc = glGetUniformLocation(shader.programId, "view");
		unsigned int lightPosLoc =
			glGetUniformLocation(shader.programId, "lightPos");
		unsigned int lightAmLoc =
			glGetUniformLocation(shader.programId, "lightAmbientIntensity");
		unsigned int lightDifLoc =
			glGetUniformLocation(shader.programId, "lightDiffuseIntensity");
		unsigned int lightSpecLoc =
			glGetUniformLocation(shader.programId, "lightSpecularIntensity");
		unsigned int camPosLoc =
			glGetUniformLocation(shader.programId, "camPos");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.getDataPtr());
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.getDataPtr());
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"),
						   1, GL_FALSE, projection.getDataPtr());

		glUniform3fv(lightPosLoc, 1, &light.pos.x);
		glUniform3fv(lightAmLoc, 1, &light.ambientIntensity.x);
		glUniform3fv(lightDifLoc, 1, &light.diffuseIntensity.x);
		glUniform3fv(lightSpecLoc, 1, &light.specularIntensity.x);

		glUniform3fv(camPosLoc, 1, &camera.pos.x);

		factor += (!isTextured * 0.001f) + (isTextured * -0.001f);

		if (isTextured && factor <= 0.0f) {
			factor = 0.0f;
		}
		else if (!isTextured && factor >= 1.0f)
			factor = 1.0f;

		this->shader.setUniform("factor", factor);
		glBindVertexArray(VAO);
		// glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
