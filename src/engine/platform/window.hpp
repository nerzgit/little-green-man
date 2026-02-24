#ifndef WINDOW_HPP
#define WINDOW_HPP

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <functional>

class Window {
public:
	Window(int width, int height, const char *title);
	~Window();

	void        run();
	void        setUpdateCallback(std::function<void(float)> callback);
	void        setRenderCallback(std::function<void()> callback);
	GLFWwindow *getGLFWWindow() {
		return window_;
	}
	int getWidth() const {
		return width_;
	}
	int getHeight() const {
		return height_;
	}

private:
	GLFWwindow                *window_;
	int                        width_;
	int                        height_;
	std::function<void(float)> updateCallback_;
	std::function<void()>      renderCallback_;

	void        initGlfw();
	void        initGlfwWindow(int width, int height, const char *title);
	static void framebufferSizeCallback(GLFWwindow *window, int width,
	                                    int height);
};

#endif // WINDOW_HPP
