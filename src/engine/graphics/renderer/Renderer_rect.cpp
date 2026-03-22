#include "../Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Renderer::drawRect(const glm::vec2& position, float width, float height,
                        const glm::vec3& color) {
	float halfWidth  = width / 2.0f;
	float halfHeight = height / 2.0f;

	float vertices[] = {
	  position.x - halfWidth, position.y - halfHeight, 0.0f, 0.0f,
	  position.x + halfWidth, position.y - halfHeight, 1.0f, 0.0f,
	  position.x + halfWidth, position.y + halfHeight, 1.0f, 1.0f,
	  position.x - halfWidth, position.y + halfHeight, 0.0f, 1.0f,
	};

	textureShader_->use();
	textureShader_->setVec4("uColor", color.r, color.g, color.b, 1.0f);
	textureShader_->setInt("tex", 0);
	whiteTexture_->bind(0);

	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::drawRectGlow(const glm::vec2& position, float width,
                            float height, const glm::vec3& color, float alpha) {
	float halfWidth  = width / 2.0f;
	float halfHeight = height / 2.0f;

	float vertices[] = {
	  position.x - halfWidth, position.y - halfHeight, 0.0f, 0.0f,
	  position.x + halfWidth, position.y - halfHeight, 1.0f, 0.0f,
	  position.x + halfWidth, position.y + halfHeight, 1.0f, 1.0f,
	  position.x - halfWidth, position.y + halfHeight, 0.0f, 1.0f,
	};

	textureShader_->use();
	textureShader_->setVec4("uColor", color.r, color.g, color.b, alpha);
	textureShader_->setInt("tex", 0);
	whiteTexture_->bind(0);

	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	// 加算合成に切り替えて描画し、元に戻す
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::drawRectHUD(const glm::vec2& position, float width, float height,
                           const glm::vec3& color, float alpha) {
	glm::mat4 hudProjection =
	  glm::ortho(0.0f, static_cast<float>(windowWidth_),
	             static_cast<float>(windowHeight_), 0.0f);

	textureShader_->use();
	textureShader_->setMat4("projection", hudProjection);
	textureShader_->setVec4("uColor", color.r, color.g, color.b, alpha);
	textureShader_->setInt("tex", 0);
	whiteTexture_->bind(0);

	float halfW      = width / 2.0f;
	float halfH      = height / 2.0f;
	float vertices[] = {
	  position.x - halfW, position.y - halfH, 0.0f, 0.0f,
	  position.x + halfW, position.y - halfH, 1.0f, 0.0f,
	  position.x + halfW, position.y + halfH, 1.0f, 1.0f,
	  position.x - halfW, position.y + halfH, 0.0f, 1.0f,
	};
	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	textureShader_->setMat4("projection", worldProjection_);
}

void Renderer::drawParallaxHUD(const Texture& texture, int texWidth,
                               int windowHeight, float uvOffsetX) {
	float winW = static_cast<float>(windowWidth_);
	float winH = static_cast<float>(windowHeight_);
	float texW = static_cast<float>(texWidth);
	float h    = static_cast<float>(windowHeight);

	float centerX    = winW / 2.0f;
	float halfW      = texW / 2.0f;
	float left       = centerX - halfW;
	float right      = centerX + halfW;
	float u0         = uvOffsetX;
	float u1         = uvOffsetX + 1.0f;
	float vertices[] = {
	  left,  0.0f, u0, 0.0f,
	  right, 0.0f, u1, 0.0f,
	  right, h,    u1, 1.0f,
	  left,  h,    u0, 1.0f,
	};

	glm::mat4 hudProjection = glm::ortho(0.0f, winW, winH, 0.0f);

	textureShader_->use();
	textureShader_->setMat4("projection", hudProjection);
	textureShader_->setVec4("uColor", 1.0f, 1.0f, 1.0f, 1.0f);
	textureShader_->setInt("tex", 0);
	texture.bind(0);

	// タイリングのため一時的に GL_REPEAT を設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 元に戻す
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	textureShader_->setMat4("projection", worldProjection_);
}
