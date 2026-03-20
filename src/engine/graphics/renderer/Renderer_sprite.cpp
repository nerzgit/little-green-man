#include "../Renderer.hpp"

void Renderer::drawSprite(const Texture& texture, const glm::vec2& position,
                          float width, float height, SpriteAnchor anchor,
                          glm::vec2 uvMin, glm::vec2 uvMax) {
	// X 方向オフセット: LEFT → 左端がposition.x, RIGHT → 右端がposition.x, それ以外 → 中央
	float xOff = hasAnchor(anchor, SpriteAnchor::LEFT)  ?  0.0f
	           : hasAnchor(anchor, SpriteAnchor::RIGHT)  ? -width
	           : -width / 2.0f;

	// Y 方向オフセット: TOP → 上端がposition.y, BOTTOM → 下端がposition.y, それ以外 → 中央
	float yOff = hasAnchor(anchor, SpriteAnchor::TOP)    ?  0.0f
	           : hasAnchor(anchor, SpriteAnchor::BOTTOM) ? -height
	           : -height / 2.0f;

	float x0 = position.x + xOff;
	float y0 = position.y + yOff;
	float x1 = x0 + width;
	float y1 = y0 + height;

	float vertices[] = {
	  x0, y0, uvMin.x, uvMin.y, x1, y0, uvMax.x, uvMin.y,
	  x1, y1, uvMax.x, uvMax.y, x0, y1, uvMin.x, uvMax.y,
	};

	textureShader_->use();
	textureShader_->setVec4("uColor", 1.0f, 1.0f, 1.0f, 1.0f);
	textureShader_->setInt("tex", 0);
	texture.bind(0);

	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::drawTile(const Texture& texture, const glm::vec2& position,
                        float width, float height, glm::vec2 uvMin,
                        glm::vec2 uvMax) {
	float halfWidth  = width / 2.0f;
	float halfHeight = height / 2.0f;

	float vertices[] = {
	  position.x - halfWidth, position.y - halfHeight, uvMin.x, uvMin.y,
	  position.x + halfWidth, position.y - halfHeight, uvMax.x, uvMin.y,
	  position.x + halfWidth, position.y + halfHeight, uvMax.x, uvMax.y,
	  position.x - halfWidth, position.y + halfHeight, uvMin.x, uvMax.y,
	};

	textureShader_->use();
	textureShader_->setVec4("uColor", 1.0f, 1.0f, 1.0f, 1.0f);
	textureShader_->setInt("tex", 0);
	texture.bind(0);

	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
