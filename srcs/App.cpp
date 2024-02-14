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
		GLFW_KEY_ESCAPE, [this]() { this->closeWindow(); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_T, [this]() { this->polygonModeHandling(); }, PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_SPACE,
		[this]() { this->features[TEXTURE] = !this->features[TEXTURE]; },
		PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_R,
		[this]() { this->features[ROTATION] = !this->features[ROTATION]; },
		PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_W, [this]() { this->moveCamera(FORWARD, 1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_S, [this]() { this->moveCamera(FORWARD, -1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_A, [this]() { this->moveCamera(LEFT, -1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_D, [this]() { this->moveCamera(LEFT, 1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_LEFT_SHIFT, [this]() { this->moveCamera(UP, 1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_LEFT_CONTROL, [this]() { this->moveCamera(UP, -1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_LEFT, [this]() { this->rotateCamera(LEFT, -1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_RIGHT, [this]() { this->rotateCamera(LEFT, 1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_UP, [this]() { this->rotateCamera(UP, 1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_DOWN, [this]() { this->rotateCamera(UP, -1); }, PRESSED);

	this->keyManager.registerCallback(
		GLFW_KEY_F, [this]() { this->resetCamera(); }, PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_G,
		[this]() { this->features[SKYBOX] = !this->features[SKYBOX]; },
		PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_C, [this]() { this->reflectModeHandling(); }, PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_U, [this]() { this->toggleUVMapping(); }, PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_B,
		[this]() { this->features[BOUND_BOX] = !this->features[BOUND_BOX]; },
		PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_N,
		[this]() { this->features[NORMALS] = !this->features[NORMALS]; },
		PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_F1, [this]() { this->skybox.setCurTex(0); }, PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_F2, [this]() { this->skybox.setCurTex(1); }, PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_F3, [this]() { this->skybox.setCurTex(2); }, PRESSED_ONCE);

	this->keyManager.registerCallback(
		GLFW_KEY_F4, [this]() { this->skybox.setCurTex(3); }, PRESSED_ONCE);
}

void App::initWindow() {
	if (!glfwInit()) {
		throw std::runtime_error("Cant init OpenGL !");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	this->sizeVec = {1280, 720};
	this->window =
		glfwCreateWindow(this->sizeVec.x, this->sizeVec.y, "scop", NULL, NULL);
	if (!window) {
		throw std::runtime_error("Cant init Window !");
	}
	glfwMakeContextCurrent(this->window);
	// glfwSwapInterval(0);

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
	this->light = Light({-4, 6, -2}, {0.9, 0.9, 0.9});
	this->light.initMatrixes();
	std::memset(&this->features, 0, sizeof(this->features));
	this->features[TEXTURE] = true;
	this->features[ROTATION] = true;
	this->features[SKYBOX] = true;
	this->shadowMap.init();
	this->skybox.init();
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
	++this->polygonMode;
	this->polygonMode %= 3;
	glPolygonMode(GL_FRONT_AND_BACK, this->modes[this->polygonMode]);
}

void App::reflectModeHandling() {
	++this->reflectMode;
	this->reflectMode %= 3;
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
	this->shader.setUniform("lightPos", this->getLightSource().pos);
	this->shader.setUniform("lightAmbientIntensity",
							this->getLightSource().ambientIntensity);
	this->shader.setUniform("lightDiffuseIntensity",
							this->getLightSource().diffuseIntensity);
	this->shader.setUniform("lightSpecularIntensity",
							this->getLightSource().specularIntensity);

	this->shader.setUniform("view", view);
	this->shader.setUniform("projection", proj);
	this->shader.setUniform("camPos", this->camera.pos);
	this->shader.setUniform("lightSpaceMatrix",
							this->getLightSource().getSpace());
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

// Object planeObj;

void App::computeShadowMap() {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	this->shadowMap.getShader().use();
	this->shadowMap.getShader().setUniform("lightProj",
										   this->getLightSource().getSpace());
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
	if (this->object.getCenter() != vec3f(0, 0, 0))
		this->object.translate({-object.getCenter().x, -object.getMinCoord().y,
								-object.getCenter().z});

	this->camera.setStartingPos({0, this->object.getCenter().y, 0},
								object.getBounds());
	this->camera.reset();
	this->box.create(this->object);
}

Light const &App::getLightSource() const {
	return (!this->features[SKYBOX] ? this->light : this->skybox.getLight());
}

void App::run() {
	// File3D plane;
	// Object planeObj;
	// plane.load("models/base.obj");
	// planeObj.configFromFile(plane);
	// planeObj.translate(this->object.getCenter());

	// File3D cube;
	// Object lig;
	// cube.load("models/cube.obj");
	// lig.configFromFile(cube);
	// lig.translate({-4, 6, -2});
	this->object.rotate(180, {0, 1, 0});

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

		if (this->features[SKYBOX]) {
			this->skybox.draw(this->camera, this->sizeVec,
							  this->modes[this->polygonMode]);
		}

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

		// lig.draw(this->shader);

		if (this->features[NORMALS]) {
			this->normalsShader.use();
			this->normalsShader.setUniform("view", view);
			this->normalsShader.setUniform("projection", projection);
			this->object.draw(this->normalsShader);
		}
		// planeObj.draw(this->shader);

		// this->viewDebugShadow();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

App::~App() {
	glfwTerminate();
}
