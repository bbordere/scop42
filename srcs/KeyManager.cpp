#include "KeyManager.hpp"

KeyState::KeyState(): isPressed(false), wasPressed(false) {}

void KeyManager::updateKeysStates(GLFWwindow *window) {
	for (auto &[key, state] : this->keysStates) {
		state.wasPressed = state.isPressed;
		state.isPressed = glfwGetKey(window, key) == GLFW_PRESS;
	}
}

void KeyManager::registerCallback(int key, std::function<void()> callback,
								  KEY_MODE mode) {
	this->keysStates[key] = KeyState();
	this->keysStates[key].callback = callback;
	this->keysStates[key].mode = mode;
}

void KeyManager::executeActions() {
	for (std::map<int, KeyState>::iterator it = this->keysStates.begin();
		 it != this->keysStates.end(); ++it)
	{
		KeyState &state = (*it).second;
		bool actiCond = false;
		if (state.mode == PRESSED)
			actiCond = state.isPressed;
		else if (state.mode == PRESSED_ONCE)
			actiCond = !state.isPressed && state.wasPressed;
		else if (state.mode == RELEASED)
			actiCond = !state.isPressed;
		if (actiCond)
			state.callback();
	}
}
