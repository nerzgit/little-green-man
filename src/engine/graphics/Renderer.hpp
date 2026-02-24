#ifndef RENDERER_HPP
#define RENDERER_HPP

// clang-format off
#include <glad/glad.h>
// clang-format on

#include "Shader.hpp"
#include <glm/glm.hpp>
#include <memory>

class Renderer {
public:
	Renderer(int windowWidth, int windowHeight);
	~Renderer();

	void clear();
	void drawCircle(const glm::vec2& position, float radius, float r, float g,
	                float b);
	void drawRect(const glm::vec2& position, float width, float height, float r,
	              float g, float b);

private:
	int                     windowWidth_;
	int                     windowHeight_;
	std::unique_ptr<Shader> shader_;
	GLuint                  VAO_, VBO_;

	void initShader();
	void initBuffers();
};

#endif // RENDERER_HPP
