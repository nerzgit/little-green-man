#ifndef RENDERER_HPP
#define RENDERER_HPP

// clang-format off
#include <glad/glad.h>
// clang-format on

#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

class FontAtlas;

class Renderer {
public:
	Renderer(int windowWidth, int windowHeight);
	~Renderer();

	void clear();
	void drawSprite(const Texture& texture, const glm::vec2& position,
	                float width, float height);
	void setCamera(const glm::vec2& cameraPos);

	// ワールド座標でテキストを描画（カメラに追従）
	// position: ベースラインの左端（ワールド座標）
	void drawText(const FontAtlas& font, const std::string& utf8Text,
	              const glm::vec2& position, float scale = 1.0f,
	              const glm::vec3& color = { 1.0f, 1.0f, 1.0f });

	// スクリーン座標でテキストを描画（カメラに追従しない HUD 用）
	// position: ウィンドウ左上を (0,0) としたピクセル座標
	void drawTextHUD(const FontAtlas& font, const std::string& utf8Text,
	                 const glm::vec2& screenPos, float scale = 1.0f,
	                 const glm::vec3& color = { 1.0f, 1.0f, 1.0f });

private:
	int                     windowWidth_;
	int                     windowHeight_;
	float                   zoom_;
	glm::mat4               worldProjection_;
	std::unique_ptr<Shader> textureShader_;
	GLuint                  textureVAO_, textureVBO_;

	void initTextureShader();
	void initTextureBuffers();
	void drawGlyphQuad(const Texture& texture, glm::vec2 topLeft, glm::vec2 size,
	                   glm::vec2 uvMin, glm::vec2 uvMax);
	void drawTextImpl(const FontAtlas& font, const std::string& utf8Text,
	                  const glm::vec2& position, float scale,
	                  const glm::vec3& color);
};

#endif // RENDERER_HPP
