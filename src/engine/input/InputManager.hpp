#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

// clang-format off
#include <GLFW/glfw3.h>
// clang-format on

class InputManager {
public:
	static void init(GLFWwindow* window);

	static bool isKeyPressed(int key);
	static bool isMouseButtonPressed(int button);
	static void getMousePosition(double& x, double& y);

private:
	static GLFWwindow* window_;
};

#endif // INPUT_MANAGER_HPP
