#include "InputManager.hpp"

GLFWwindow* InputManager::window_ = nullptr;

void InputManager::init(GLFWwindow* window) {
	window_ = window;
}

bool InputManager::isKeyPressed(int key) {
	if (window_) {
		return glfwGetKey(window_, key) == GLFW_PRESS;
	}
	return false;
}

bool InputManager::isMouseButtonPressed(int button) {
	if (window_) {
		return glfwGetMouseButton(window_, button) == GLFW_PRESS;
	}
	return false;
}

void InputManager::getMousePosition(double& x, double& y) {
	if (window_) {
		glfwGetCursorPos(window_, &x, &y);
	}
}
