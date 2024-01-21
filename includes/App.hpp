#pragma once

#include "BmpImage.hpp"
#include "Camera.hpp"
#include "File3D.hpp"
#include "KeyManager.hpp"
#include "Light.hpp"
#include "Mat4.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "ShadowMap.hpp"
#include "SkyBox.hpp"
#include "Vectors.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <exception>
#include <iostream>

class App {
	private:
		GLFWwindow *window;
		Shader shader;
		Camera camera;
		KeyManager keyManager;
		File3D model3d;
		SkyBox skybox;
		ShadowMap shadowMap;
		Shader chromeShader;

		bool isSpacePressed;
		bool isTextured = true;
		bool isRotating = true;
		bool isSkyboxed = true;
		bool isChromed = false;
		float delta = 0.0f;
		float modelRotationAngle = 90.0f;
		float blendingFActor = 0.0f;
		int polygonMode = 0;

		unsigned int width, height;

		GLenum const modes[3] = {GL_FILL, GL_LINE, GL_POINT};

	public:
		App();
		void init(std::string const &path, std::string const &texture);
		void run();

	private:
		void closeWindow();
		void polygonModeHandling();
		void moveCamera(DIRECTION dir, int factor);
		void rotateCamera(DIRECTION dir, int factor);
		void resetCamera();
		void setupLight();
		void setRenderUniforms(Light const &light, mat4f const &view,
							   mat4f const &proj);
		void computeRendering(mat4f &model, Light const &light);
		void computeShadowMap(Light const &light, Object const &object);
		void fpsUpdate();
		void viewDebugShadow();
		void toggleBoolean(bool *val);
};
