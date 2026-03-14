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

// drawSprite の起点を指定するビットフラグ
// 例: SpriteAnchor::LEFT | SpriteAnchor::TOP → 左上を起点にする
// X/Y どちらも指定しない場合は中央が起点
enum class SpriteAnchor : int {
	CENTER = 0,
	LEFT   = 1 << 0,
	RIGHT  = 1 << 1,
	TOP    = 1 << 2,
	BOTTOM = 1 << 3,
};
inline SpriteAnchor operator|(SpriteAnchor a, SpriteAnchor b) {
	return static_cast<SpriteAnchor>(static_cast<int>(a) | static_cast<int>(b));
}
inline bool hasAnchor(SpriteAnchor flags, SpriteAnchor flag) {
	return (static_cast<int>(flags) & static_cast<int>(flag)) != 0;
}

class Renderer {
public:
	Renderer(int windowWidth, int windowHeight);
	~Renderer();

	void clear();
	void drawSprite(const Texture& texture, const glm::vec2& position,
	                float width, float height,
	                SpriteAnchor anchor = SpriteAnchor::CENTER,
	                glm::vec2    uvMin  = {0.0f, 0.0f},
	                glm::vec2    uvMax  = {1.0f, 1.0f});
	void drawTile(const Texture& texture, const glm::vec2& position,
	              float width, float height,
	              glm::vec2 uvMin, glm::vec2 uvMax);
	void drawRect(const glm::vec2& position, float width, float height,
	              const glm::vec3& color);
	// 加算合成で描画（グロー効果用）
	void drawRectGlow(const glm::vec2& position, float width, float height,
	                  const glm::vec3& color, float alpha);
	// スクリーン座標で矩形を描画（カメラに追従しない HUD 用）
	// position: 矩形の中心（ウィンドウ左上を (0,0) としたピクセル座標）
	void drawRectHUD(const glm::vec2& position, float width, float height,
	                 const glm::vec3& color, float alpha = 1.0f);
	// テクスチャを画面全体に貼り、uvOffsetX でX方向にスクロール（GL_REPEAT）
	void drawParallaxHUD(const Texture& texture, int windowWidth,
	                     int windowHeight, float uvOffsetX);
	void setCamera(const glm::vec2& cameraPos);

	// ワールド座標でテキストを描画（カメラに追従）
	// position: ベースラインの左端（ワールド座標）
	void drawText(const FontAtlas& font, const std::string& utf8Text,
	              const glm::vec2& position, float scale = 1.0f,
	              const glm::vec3& color = {1.0f, 1.0f, 1.0f});

	// スクリーン座標でテキストを描画（カメラに追従しない HUD 用）
	// position: ウィンドウ左上を (0,0) としたピクセル座標
	void drawTextHUD(const FontAtlas& font, const std::string& utf8Text,
	                 const glm::vec2& screenPos, float scale = 1.0f,
	                 const glm::vec3& color = {1.0f, 1.0f, 1.0f});

private:
	int                      windowWidth_;
	int                      windowHeight_;
	float                    zoom_;
	glm::mat4                worldProjection_;
	std::unique_ptr<Shader>  textureShader_;
	std::unique_ptr<Texture> whiteTexture_; // drawRect 用 1x1 白テクスチャ
	GLuint                   textureVAO_, textureVBO_;

	void initTextureShader();
	void initTextureBuffers();
	void drawGlyphQuad(const Texture& texture, glm::vec2 topLeft,
	                   glm::vec2 size, glm::vec2 uvMin, glm::vec2 uvMax);
	void drawTextImpl(const FontAtlas& font, const std::string& utf8Text,
	                  const glm::vec2& position, float scale,
	                  const glm::vec3& color);
};

#endif // RENDERER_HPP
