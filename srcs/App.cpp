#include "App.hpp"

App::App() {}

void App::closeWindow() {
	glfwSetWindowShouldClose(this->window, true);
}

void App::toggleUVMapping() {
	static int mode = 0;
	mode = (mode + 1) % 2;
	this->shader.use();
	this->shader.setUniform("uvMappingMode", static_cast<int>(mode));
}

void App::initKeysCallbacks() {
	this->keyManager.registerCallback(
		GLFW_KEY_ESCAPE,
		std::function<void()>{std::bind(&App::closeWindow, this)}, PRESSED);
	this->keyManager.registerCallback(
		GLFW_KEY_T,
		std::function<void()>{std::bind(&App::polygonModeHandling, this)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_SPACE,
		std::function<void()>{
			std::bind(&App::toggleBoolean, this, &this->features[TEXTURE])},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_R,
		std::function<void()>{
			std::bind(&App::toggleBoolean, this, &this->features[ROTATION])},
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
	this->keyManager.registerCallback(
		GLFW_KEY_G,
		std::function<void()>{
			std::bind(&App::toggleBoolean, this, &this->features[SKYBOX])},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_C,
		std::function<void()>{std::bind(&App::reflectModeHandling, this)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_U,
		std::function<void()>{std::bind(&App::toggleUVMapping, this)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_B,
		std::function<void()>{
			std::bind(&App::toggleBoolean, this, &this->features[BOUND_BOX])},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_N,
		std::function<void()>{
			std::bind(&App::toggleBoolean, this, &this->features[NORMALS])},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_F1,
		std::function<void()>{
			std::bind(&App::setValue, this, &this->skybox.curTex, 0)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_F2,
		std::function<void()>{
			std::bind(&App::setValue, this, &this->skybox.curTex, 1)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_F3,
		std::function<void()>{
			std::bind(&App::setValue, this, &this->skybox.curTex, 2)},
		PRESSED_ONCE);
	this->keyManager.registerCallback(
		GLFW_KEY_F4,
		std::function<void()>{
			std::bind(&App::setValue, this, &this->skybox.curTex, 3)},
		PRESSED_ONCE);
}

void App::initWindow() {
	if (!glfwInit()) {
		throw std::runtime_error("Cant init OpenGL !");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	this->sizeVec = {500, 500};
	this->window =
		glfwCreateWindow(this->sizeVec.x, this->sizeVec.y, "scop", NULL, NULL);
	if (!window) {
		throw std::runtime_error("Cant init Window !");
	}
	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(0);

	glfwSetFramebufferSizeCallback(window, resizeHandler);
	glfwSetDropCallback(window, dropHandler);

	this->userPointers[0] = &this->sizeVec;
	this->userPointers[1] = &this->curObjPath;
	this->userPointers[2] = &this->curTexPath;

	glfwSetWindowUserPointer(this->window, this->userPointers);
}

void App::setValue(std::size_t *ptr, std::size_t val) {
	*ptr = val;
}

void App::init(std::string const &path, std::string const &texturePath) {

	this->initWindow();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		throw std::runtime_error("Cant init GLAD !");
	}

	this->initObject(path);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	this->shader = Shader("shaders/default.frag", "shaders/default.vert");
	this->initKeysCallbacks();

	this->reflectShader =
		Shader("shaders/reflect.frag", "shaders/reflect.vert");
	this->normalsShader = Shader("shaders/normals.frag", "shaders/normals.vert",
								 "shaders/normals.geom");
	this->textures[0].loadFromFile(texturePath);
	this->light = Light({3, 2, 4}, {0.98, 0.92, 0.96});
	// this->light = Light({0, 0, 4}, {0.98, 0.92, 0.96});
	this->light.initMatrixes();
	std::memset(&this->features, 0, sizeof(this->features));
	this->features[TEXTURE] = true;
	this->features[ROTATION] = true;
	this->features[SKYBOX] = true;
}

void App::toggleBoolean(bool *val) {
	*val = !*val;
}

void App::rotateCamera(DIRECTION dir, int factor) {
	float rotaSpeed = 40.0f * this->delta;
	switch (dir) {
		case LEFT:
			this->camera.rotation.y += rotaSpeed * factor;
			break;
		case UP:
			if (factor == 1)
				this->camera.rotation.x =
					std::min(this->camera.rotation.x + rotaSpeed, 89.0f);
			else
				this->camera.rotation.x =
					std::max(this->camera.rotation.x - rotaSpeed, -89.0f);
			break;
		default:
			break;
	}
}

void App::polygonModeHandling() {
	this->polygonMode = (++this->polygonMode) % 3;
	glPolygonMode(GL_FRONT_AND_BACK, this->modes[this->polygonMode]);
}

void App::reflectModeHandling() {
	this->reflectMode = (++this->reflectMode) % 3;
}

void App::resetCamera() {
	this->camera.reset();
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

void App::setRenderUniforms(mat4f const &view, mat4f const &proj) {
	this->shader.setUniform("lightPos", light.pos);
	this->shader.setUniform("lightAmbientIntensity", light.ambientIntensity);
	this->shader.setUniform("lightDiffuseIntensity", light.diffuseIntensity);
	this->shader.setUniform("lightSpecularIntensity", light.specularIntensity);
	this->shader.setUniform("view", view);
	this->shader.setUniform("projection", proj);
	this->shader.setUniform("camPos", this->camera.pos);
	this->shader.setUniform("lightSpaceMatrix", this->light.getSpace());
	this->shader.setUniform("shadowMap", 1);
	this->shader.setUniform("scale", this->object.getScalingFactors().x);
}

void App::computeRendering() {

	bool isTextured = this->features[TEXTURE];
	this->camera.rotationHandling();

	this->blendingFActor += (!isTextured * 0.001f) + (isTextured * -0.001f);

	if (isTextured && this->blendingFActor <= 0.0f) {
		this->blendingFActor = 0.0f;
	}
	else if (!isTextured && this->blendingFActor >= 1.0f)
		this->blendingFActor = 1.0f;

	this->shader.setUniform("factor", this->blendingFActor);
}

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// Object planeObj;

void App::computeShadowMap() {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	this->shadowMap.getShader().use();
	this->shadowMap.getShader().setUniform("lightProj", light.getSpace());
	glViewport(0, 0, SHADOW_RES, SHADOW_RES);
	glBindFramebuffer(GL_FRAMEBUFFER, this->shadowMap.getFbo());
	glClear(GL_DEPTH_BUFFER_BIT);
	this->object.draw(this->shadowMap.getShader());
	glDisable(GL_CULL_FACE);

	// glEnable(GL_CULL_FACE);
}

void App::fpsUpdate() {
	static float lastFrame = 0.0f;
	static float prevUpdate = glfwGetTime();
	float currentFrame = glfwGetTime();
	this->delta = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if (currentFrame - prevUpdate >= 1.0) {
		std::string FPS =
			"scop - " + std::to_string((int)((1000 / this->delta) / 1000));
		FPS += " FPS";
		glfwSetWindowTitle(window, FPS.c_str());
		prevUpdate = currentFrame;
	}
}

void App::viewDebugShadow() {
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	Shader debug("shaders/debug.frag", "shaders/debug.vert");
	debug.use();
	debug.setUniform("depthMap", 1);
	if (quadVAO == 0) {
		// clang-format off
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 
			1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// clang-format on

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
					 GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
							  (void *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
							  (void *)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void App::dropFileHandler() {
	static std::string objPath = this->curObjPath;
	static std::string texPath = this->curTexPath;
	if (objPath != this->curObjPath) {
		this->initObject(this->curObjPath);
		objPath = this->curObjPath;
	}

	if (texPath != this->curTexPath) {
		this->textures[0].loadFromFile(this->curTexPath);
		texPath = this->curTexPath;
	}
}

void App::initObject(std::string const &path) {
	this->model3d.load(path);
	this->object.configFromFile(this->model3d);
	this->curObjPath = path;
	// std::cout << "BOUND" << this->model3d.getBoundVec() << '\n';
	// std::cout << "CENTER" << this->model3d.getCenter() << '\n';
	// std::cout << "MIN " << this->model3d.getMinCoord() << '\n';

	if (this->object.getCenter() != vec3f(0, 0, 0))
		this->object.translate({-object.getCenter().x, -object.getMinCoord().y,
								-object.getCenter().z});

	this->camera.setStartingPos({0, this->object.getCenter().y, 0},
								object.getBounds());
	this->camera.reset();
	this->box.create(this->object);
}

void App::run() {

	glUseProgram(0);
	this->shadowMap.init();
	this->skybox.init();

	// File3D plane;
	// plane.load("models/base.obj");
	// planeObj.configFromFile(plane);
	// planeObj.translate({0, -2, 0});
	// planeObj.scale({0.05, 0.05, 0.05});
	// planeObj.translate(this->object.getCenter());

	while (!glfwWindowShouldClose(this->window)) {
		this->dropFileHandler();
		this->fpsUpdate();
		this->keyManager.updateKeysStates(window);
		this->keyManager.executeActions();

		glClearColor(0.07f, 0.08f, 0.13f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (this->features[ROTATION])
			this->object.rotate(-15 * this->delta, vec3f(0, 1, 0));

		this->computeShadowMap();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, this->sizeVec.x, this->sizeVec.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->shader.use();

		mat4f projection = mat4f::makePerspective(
			45.0f, (float)this->sizeVec.x / (float)this->sizeVec.y, 0.1f,
			250.0f);
		mat4f view = mat4f::lookAt(this->camera.pos,
								   this->camera.pos + this->camera.target,
								   this->camera.up);

		this->setRenderUniforms(view, projection);
		this->computeRendering();

		glActiveTexture(GL_TEXTURE0);
		this->textures[0].bind();
		glActiveTexture(GL_TEXTURE1);
		this->shadowMap.bindTexture();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, this->sizeVec.x, this->sizeVec.y);

		if (this->features[BOUND_BOX])
			this->box.draw(projection, view, this->object.getModel());

		if (this->reflectMode) {
			this->reflectShader.use();
			this->reflectShader.setUniform("view", view);
			this->reflectShader.setUniform("cameraPos", this->camera.pos);
			this->reflectShader.setUniform("projection", projection);
			this->reflectShader.setUniform("mode", this->reflectMode);
			this->object.draw(this->reflectShader);
		}
		else
			this->object.draw(this->shader);

		if (this->features[NORMALS]) {
			this->normalsShader.use();
			this->normalsShader.setUniform("view", view);
			this->normalsShader.setUniform("projection", projection);
			this->object.draw(this->normalsShader);
		}
		// planeObj.draw(this->shader);

		// this->viewDebugShadow();
		this->skybox.draw(this->camera, this->features[SKYBOX], this->sizeVec.x,
						  this->sizeVec.y, this->modes[this->polygonMode]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
