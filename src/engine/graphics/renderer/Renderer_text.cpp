#include "../Renderer.hpp"

#include "../FontAtlas.hpp"
#include <glm/gtc/matrix_transform.hpp>

// UTF-8 文字列から次の codepoint を取得し、ポインタを進める
static uint32_t nextCodepoint(const char*& p) {
	auto c = static_cast<unsigned char>(*p++);
	if (c < 0x80)
		return c;
	if ((c & 0xE0) == 0xC0) {
		return ((c & 0x1F) << 6) | (static_cast<unsigned char>(*p++) & 0x3F);
	}
	if ((c & 0xF0) == 0xE0) {
		uint32_t cp = static_cast<uint32_t>(c & 0x0F) << 12;
		cp |= (static_cast<unsigned char>(*p++) & 0x3F) << 6;
		cp |= (static_cast<unsigned char>(*p++) & 0x3F);
		return cp;
	}
	// 4バイト (絵文字など)
	uint32_t cp = static_cast<uint32_t>(c & 0x07) << 18;
	cp |= (static_cast<unsigned char>(*p++) & 0x3F) << 12;
	cp |= (static_cast<unsigned char>(*p++) & 0x3F) << 6;
	cp |= (static_cast<unsigned char>(*p++) & 0x3F);
	return cp;
}

void Renderer::drawGlyphQuad(const Texture& texture, glm::vec2 topLeft,
                             glm::vec2 size, glm::vec2 uvMin, glm::vec2 uvMax) {
	float vertices[] = {
	  topLeft.x,          topLeft.y,          uvMin.x, uvMin.y,
	  topLeft.x + size.x, topLeft.y,          uvMax.x, uvMin.y,
	  topLeft.x + size.x, topLeft.y + size.y, uvMax.x, uvMax.y,
	  topLeft.x,          topLeft.y + size.y, uvMin.x, uvMax.y,
	};

	textureShader_->setInt("tex", 0);
	texture.bind(0);

	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::drawTextImpl(const FontAtlas& font, const std::string& utf8Text,
                            const glm::vec2& position, float scale,
                            const glm::vec3& color) {
	textureShader_->use();
	textureShader_->setVec4("uColor", color.r, color.g, color.b, 1.0f);

	glm::vec2   cursor = position;
	const char* p      = utf8Text.c_str();

	while (*p) {
		uint32_t cp = nextCodepoint(p);
		if (cp == '\n') {
			cursor.x = position.x;
			cursor.y += font.getLineHeight() * scale;
			continue;
		}
		const auto* g = font.getGlyph(cp);
		if (!g)
			continue;

		glm::vec2 topLeft = {cursor.x + g->bearing.x * scale,
		                     cursor.y + g->bearing.y * scale};
		glm::vec2 size    = g->size * scale;

		drawGlyphQuad(font.getTexture(), topLeft, size, g->uvMin, g->uvMax);

		cursor.x += g->advance * scale;
	}
}

void Renderer::drawText(const FontAtlas& font, const std::string& utf8Text,
                        const glm::vec2& position, float scale,
                        const glm::vec3& color) {
	drawTextImpl(font, utf8Text, position, scale, color);
}

void Renderer::drawTextHUD(const FontAtlas& font, const std::string& utf8Text,
                           const glm::vec2& screenPos, float scale,
                           const glm::vec3& color) {
	// HUD 用プロジェクション（ウィンドウピクセル座標）に切り替え
	glm::mat4 hudProjection =
	  glm::ortho(0.0f, static_cast<float>(windowWidth_),
	             static_cast<float>(windowHeight_), 0.0f);

	textureShader_->use();
	textureShader_->setMat4("projection", hudProjection);

	drawTextImpl(font, utf8Text, screenPos, scale, color);

	// ワールドプロジェクションに戻す
	textureShader_->setMat4("projection", worldProjection_);
}
