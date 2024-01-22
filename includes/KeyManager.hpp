#pragma once

// #include <GL/glew.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>
#include <map>

class App;

enum KEY_MODE
{
	PRESSED,
	PRESSED_ONCE,
	RELEASED
};

enum DIRECTION
{
	FORWARD,
	LEFT,
	UP
};

class KeyState {
	public:
		bool isPressed;
		bool wasPressed;
		KEY_MODE mode;
		std::function<void()> callback;

	public:
		KeyState();
};

class KeyManager {
	private:
		std::map<int, KeyState> keysStates;

	public:
		void updateKeysStates(GLFWwindow *window);
		void registerCallback(int key, std::function<void()> callback,
							  KEY_MODE mode);
		void executeActions();
};
