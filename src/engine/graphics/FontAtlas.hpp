#ifndef FONT_ATLAS_HPP
#define FONT_ATLAS_HPP

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class Texture;

class FontAtlas {
public:
	struct Glyph {
		glm::vec2 uvMin;    // アトラス内のUV左上
		glm::vec2 uvMax;    // アトラス内のUV右下
		glm::vec2 size;     // グリフのピクセルサイズ
		glm::vec2 bearing;  // カーソル位置からのオフセット (xoff, yoff)
		float     advance;  // 次の文字へのカーソル送り量
	};

	// fontPath : TTFファイルパス
	// fontSize : レンダリングサイズ（px）
	// atlasSize: アトラステクスチャの一辺のサイズ（2の累乗推奨）
	FontAtlas(const std::string& fontPath, float fontSize, int atlasSize = 512);

	const Texture& getTexture() const { return *texture_; }
	const Glyph*   getGlyph(uint32_t codepoint) const;
	float          getLineHeight() const { return lineHeight_; }
	float          getFontSize() const { return fontSize_; }

private:
	std::unique_ptr<Texture>            texture_;
	std::unordered_map<uint32_t, Glyph> glyphs_;
	float                               lineHeight_ = 0.0f;
	float                               fontSize_   = 0.0f;
};

#endif // FONT_ATLAS_HPP
