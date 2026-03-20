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

	/**
	 * ワールド座標にスプライトを描画する。
	 * anchor で position のどの端を基準にするか指定できる。
	 * uvMin/uvMax でテクスチャアトラス上の切り出し範囲を指定できる。
	 */
	void drawSprite(const Texture& texture, const glm::vec2& position,
	                float width, float height,
	                SpriteAnchor anchor = SpriteAnchor::CENTER,
	                glm::vec2    uvMin  = {0.0f, 0.0f},
	                glm::vec2    uvMax  = {1.0f, 1.0f});

	/**
	 * ワールド座標にタイルを描画する。
	 * position は矩形の中心。uvMin/uvMax でタイルセット上の UV 範囲を指定する。
	 */
	void drawTile(const Texture& texture, const glm::vec2& position,
	              float width, float height,
	              glm::vec2 uvMin, glm::vec2 uvMax);

	/**
	 * ワールド座標に不透明な矩形を描画する。
	 * デバッグ表示やライトビームなど、テクスチャなしの塗り潰しに使う。
	 */
	void drawRect(const glm::vec2& position, float width, float height,
	              const glm::vec3& color);

	/**
	 * ワールド座標に加算合成で矩形を描画する。
	 * 光のグロー効果など、背景を明るく見せる演出に使う。
	 */
	void drawRectGlow(const glm::vec2& position, float width, float height,
	                  const glm::vec3& color, float alpha);

	/**
	 * スクリーン座標に矩形を描画する（カメラに追従しない HUD 用）。
	 * position は矩形の中心で、ウィンドウ左上を (0,0) としたピクセル座標。
	 * 暗転オーバーレイや UI 背景などに使う。
	 */
	void drawRectHUD(const glm::vec2& position, float width, float height,
	                 const glm::vec3& color, float alpha = 1.0f);

	/**
	 * スクリーン全体にテクスチャをタイリング描画し、uvOffsetX で水平スクロールさせる。
	 * GL_REPEAT を一時的に有効にして視差背景（パララックス）を実現する。
	 */
	void drawParallaxHUD(const Texture& texture, int windowWidth,
	                     int windowHeight, float uvOffsetX);

	void setCamera(const glm::vec2& cameraPos, float zoom);

	/**
	 * ワールド座標に UTF-8 テキストを描画する（カメラに追従）。
	 * position はベースラインの左端（ワールド座標）。
	 */
	void drawText(const FontAtlas& font, const std::string& utf8Text,
	              const glm::vec2& position, float scale = 1.0f,
	              const glm::vec3& color = {1.0f, 1.0f, 1.0f});

	/**
	 * スクリーン座標に UTF-8 テキストを描画する（カメラに追従しない HUD 用）。
	 * screenPos はウィンドウ左上を (0,0) としたピクセル座標。
	 * ダイアログや残機表示など UI テキストに使う。
	 */
	void drawTextHUD(const FontAtlas& font, const std::string& utf8Text,
	                 const glm::vec2& screenPos, float scale = 1.0f,
	                 const glm::vec3& color = {1.0f, 1.0f, 1.0f});

private:
	int                      windowWidth_;
	int                      windowHeight_;
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
