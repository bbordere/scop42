#include "App.hpp"

App::App() {}

void App::closeWindow() {
	glfwSetWindowShouldClose(this->window, true);
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void App::init(std::string const &path, std::string const &texturePath) {
	if (!glfwInit()) {
		throw std::runtime_error("Cant init OpenGL !");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->width = 1280;
	this->height = 720;

	this->window =
		glfwCreateWindow(this->width, this->height, "scop", NULL, NULL);
	if (!window) {
		throw std::runtime_error("Cant init Window !");
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw std::runtime_error("Cant init GLEW !");
	}
	this->shader = Shader("shaders/fragment.glsl", "shaders/vertex.glsl");
	this->keyManager.registerCallback(
		GLFW_KEY_ESCAPE,
		std::function<void()>{std::bind(&App::closeWindow, this)}, PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_T,
		std::function<void()>{std::bind(&App::polygonModeHandling, this)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_SPACE,
		std::function<void()>{std::bind(&App::texturingModeHandling, this)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_R,
		std::function<void()>{std::bind(&App::rotationHandling, this)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_W,
		std::function<void()>{std::bind(&App::moveCamera, this, FORWARD, 1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_S,
		std::function<void()>{std::bind(&App::moveCamera, this, FORWARD, -1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_A,
		std::function<void()>{std::bind(&App::moveCamera, this, LEFT, -1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_D,
		std::function<void()>{std::bind(&App::moveCamera, this, LEFT, 1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_LEFT_SHIFT,
		std::function<void()>{std::bind(&App::moveCamera, this, UP, 1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_LEFT_CONTROL,
		std::function<void()>{std::bind(&App::moveCamera, this, UP, -1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_LEFT,
		std::function<void()>{std::bind(&App::rotateCamera, this, LEFT, -1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_RIGHT,
		std::function<void()>{std::bind(&App::rotateCamera, this, LEFT, 1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_UP,
		std::function<void()>{std::bind(&App::rotateCamera, this, UP, 1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_DOWN,
		std::function<void()>{std::bind(&App::rotateCamera, this, UP, -1)},
		PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_F, std::function<void()>{std::bind(&App::resetCamera, this)},
		PRESSED_ONCE);

	model3d.load(path);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data =
		stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

	// BmpImage image;
	// image.extractData("res/texture.bmp");
	// uint8_t *data = image.data.data();
	// width = image.getInfoHeader().imgWidth;
	// height = image.getInfoHeader().imgHeight;

	GLuint texture;
	glGenTextures(1, &texture);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
	// GL_MIRRORED_REPEAT); glTexParameteri(GL_TEXTURE_2D,
	// GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);
}

void App::rotateCamera(DIRECTION dir, int factor) {
	switch (dir) {
		case LEFT:
			this->camera.rotation.y += 0.1f * factor;
			break;
		case UP:
			if (factor == 1)
				this->camera.rotation.x =
					std::min(this->camera.rotation.x + 0.1f, 89.0f);
			else
				this->camera.rotation.x =
					std::max(this->camera.rotation.x - 0.1f, -89.0f);
			break;
		default:
			break;
	}
}

void App::polygonModeHandling() {
	static int mode = 0;
	mode = (++mode) % 3;
	glPolygonMode(GL_FRONT_AND_BACK, this->modes[mode]);
}

void App::resetCamera() {
	this->camera.reset();
}

void App::texturingModeHandling() {
	this->isTextured = !this->isTextured;
}

void App::rotationHandling() {
	this->isRotating = !this->isRotating;
}

void App::moveCamera(DIRECTION dir, int factor) {
	float cameraSpeed = 4.0f * this->delta;

	switch (dir) {
		case FORWARD:
			this->camera.pos += (this->camera.target * cameraSpeed) * factor;
			break;
		case LEFT:
			this->camera.pos += (vec3f::normalize(vec3f::cross(
									 this->camera.target, this->camera.up)) *
								 cameraSpeed) *
								factor;
			break;

		case UP:
			this->camera.pos += (vec3f(0, 1, 0) * cameraSpeed) * factor;
			break;
	}
}

void App::setRenderUniforms(Light const &light, mat4f const &model,
							mat4f const &view, mat4f const &proj) {
	this->shader.setUniform("lightPos", light.pos);
	this->shader.setUniform("lightAmbientIntensity", light.ambientIntensity);
	this->shader.setUniform("lightDiffuseIntensity", light.diffuseIntensity);
	this->shader.setUniform("lightSpecularIntensity", light.specularIntensity);
	this->shader.setUniform("model", model);
	this->shader.setUniform("view", view);
	this->shader.setUniform("projection", proj);
	this->shader.setUniform("camPos", this->camera.pos);
}

void App::computeRendering(float &lastFrame, mat4f &model, Light const &light) {
	float currentFrame = glfwGetTime();

	this->delta = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (this->isRotating)
		model.rotate(15 * this->delta, vec3f(0.0f, 2.0f, 0.0f));

	this->camera.rotationHandling();

	mat4f projection = mat4f::makePerspective(
		45, (float)this->width / (float)this->height, 0.1f, 100.0f);
	mat4f view =
		mat4f::lookAt(this->camera.pos, this->camera.pos + this->camera.target,
					  this->camera.up);

	this->setRenderUniforms(light, model, view, projection);

	this->blendingFActor += (!isTextured * 0.001f) + (isTextured * -0.001f);

	if (isTextured && this->blendingFActor <= 0.0f) {
		this->blendingFActor = 0.0f;
	}
	else if (!isTextured && this->blendingFActor >= 1.0f)
		this->blendingFActor = 1.0f;

	this->shader.setUniform("factor", this->blendingFActor);
}

void App::run() {

	auto faces = model3d.getFaces();

	Light light({-5, 4, 12}, {1, 1, 1});

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

			vertices.push_back(f.normal[i].x);
			vertices.push_back(f.normal[i].y);
			vertices.push_back(f.normal[i].z);
		}
	}

	// Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind VAO and VBO
	glBindVertexArray(VAO);

	// Copy vertex data to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
				 vertices.data(), GL_STATIC_DRAW);

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

	glBindVertexArray(VAO);

	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	mat4f model = mat4f::makeIdentity();
	float lastFrame = 0.0f;

	glUseProgram(0);
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;
	this->skybox.init();
	while (!glfwWindowShouldClose(this->window)) {

		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0) {
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;

			// Use this if you have disabled VSync
			// camera.Inputs(window);
		}
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->shader.use();
		this->keyManager.updateKeysStates(window);
		this->keyManager.executeActions();
		this->computeRendering(lastFrame, model, light);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDepthFunc(GL_LEQUAL);
		this->skybox.draw(this->camera, true, this->width, this->height);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
