#pragma once

#include "BmpImage.hpp"
#include "Camera.hpp"
#include "KeyManager.hpp"
#include "Light.hpp"
#include "Mat4.hpp"
#include "Model.hpp"
#include "Shader.hpp"
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
		Model model3d;
		SkyBox skybox;

		bool isSpacePressed;
		bool isTextured = true;
		bool isRotating = true;
		float delta = 0.0f;
		float modelRotationAngle = 90.0f;
		float blendingFActor = 0.0f;

		unsigned int width, height;

		GLenum const modes[3] = {GL_FILL, GL_LINE, GL_POINT};

	public:
		App();
		void init(std::string const &path, std::string const &texture);
		void run();

	private:
		void closeWindow();
		void polygonModeHandling();
		void texturingModeHandling();
		void rotationHandling();
		void moveCamera(DIRECTION dir, int factor);
		void rotateCamera(DIRECTION dir, int factor);
		void resetCamera();
		void setupLight();
		void setRenderUniforms(Light const &light, mat4f const &model,
							   mat4f const &view, mat4f const &proj);
		void computeRendering(float &lastFrame, mat4f &model,
							  Light const &light);
};
