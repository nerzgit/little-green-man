#ifndef RENDERER_HPP
#define RENDERER_HPP

// clang-format off
#include <glad/glad.h>
// clang-format on

#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <memory>

class Renderer {
public:
	Renderer(int windowWidth, int windowHeight);
	~Renderer();

	void clear();
	void drawSprite(const Texture& texture, const glm::vec2& position,
	                float width, float height);

private:
	int                     windowWidth_;
	int                     windowHeight_;
	std::unique_ptr<Shader> textureShader_;
	GLuint                  textureVAO_, textureVBO_;

	void initTextureShader();
	void initTextureBuffers();
};

#endif // RENDERER_HPP
