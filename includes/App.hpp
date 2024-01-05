#pragma once

#include "BmpImage.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Mat4.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Vectors.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <exception>
#include <iostream>

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

class App {
	private:
		GLFWwindow *window;
		Shader shader;
		Camera camera;

		bool isSpacePressed;
		bool isTextured = true;
		bool isRotating = true;
		float delta = 0.0f;
		float modelRotationAngle = 90.0f;

		GLenum const modes[3] = {GL_FILL, GL_LINE, GL_POINT};

	public:
		App();
		void init();
		void run();

	private:
		void inputHandling();
};
