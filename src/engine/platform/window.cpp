#include "window.hpp"

Window::Window(int width, int height, const char *title)
    : window_(nullptr), width_(width), height_(height) {
	initGlfw();
	initGlfwWindow(width, height, title);
}

Window::~Window() {
	if (window_) {
		glfwDestroyWindow(window_);
	}
	glfwTerminate();
}

void Window::initGlfw() {
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void Window::initGlfwWindow(int width, int height, const char *title) {
	window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window_) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window_, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);
}

void Window::framebufferSizeCallback(GLFWwindow *, int width, int height) {
	glViewport(0, 0, width, height);
}

void Window::setUpdateCallback(std::function<void(float)> callback) {
	updateCallback_ = callback;
}

void Window::setRenderCallback(std::function<void()> callback) {
	renderCallback_ = callback;
}

void Window::run() {
	float lastTime = static_cast<float>(glfwGetTime());

	while (!glfwWindowShouldClose(window_)) {
		// デルタタイムの計算
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime   = currentTime - lastTime;
		lastTime          = currentTime;

		// 入力
		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window_, true);
		}

		// 更新コールバック
		if (updateCallback_) {
			updateCallback_(deltaTime);
		}

		// 描画コールバック
		if (renderCallback_) {
			renderCallback_();
		}

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
}
