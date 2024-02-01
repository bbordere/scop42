#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "BmpImage.hpp"
#include "BoundingBox.hpp"
#include "Camera.hpp"
#include "File3D.hpp"
#include "KeyManager.hpp"
#include "Light.hpp"
#include "Mat4.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "ShadowMap.hpp"
#include "SkyBox.hpp"
#include "Texture.hpp"
#include "Vectors.hpp"

#include <cmath>
#include <exception>
#include <iostream>

enum
{
	IS_COLORED,
	IS_TEXTURED,
};

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
		Texture textures[10];
		Light light;
		Object object;
		BoundingBox box;
		Shader normalsShader;

		std::string curObjPath;

		bool isSpacePressed = false;
		bool isTextured = true;
		bool isRotating = true;
		bool isSkyboxed = true;
		bool isChromed = false;
		bool isBoxRendered = false;
		bool isNormalsRendered = false;

		float delta = 0.0f;
		float modelRotationAngle = 90.0f;
		float blendingFActor = 0.0f;
		int polygonMode = 0;

		unsigned int width, height;
		vec2u sizeVec;

		GLenum const modes[3] = {GL_FILL, GL_LINE, GL_POINT};

		void *userPointers[2];

	public:
		App();
		void init(std::string const &path, std::string const &texture);
		void run();
		void resizeWindow(GLFWwindow *window, int width, int height);

	private:
		void closeWindow();
		void polygonModeHandling();
		void moveCamera(DIRECTION dir, int factor);
		void rotateCamera(DIRECTION dir, int factor);
		void resetCamera();
		void setupLight();
		void setRenderUniforms(mat4f const &view, mat4f const &proj);
		void computeRendering();
		void computeShadowMap();
		void fpsUpdate();
		void viewDebugShadow();
		void toggleBoolean(bool *val);
		void toggleUVMapping();
		void initKeysCallbacks();
		void initWindow();
		void modelChangingHandler();
		void initObject(std::string const &path);
};
