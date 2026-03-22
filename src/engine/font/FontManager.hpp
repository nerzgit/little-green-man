#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include "FontAtlas.hpp"
#include <memory>

// ゲーム共通フォントのシングルトン管理クラス。
// 各シーンが FontAtlas を個別生成せず、このクラスから取得して使い回す。
class FontManager {
public:
	static FontManager& getInstance() {
		static FontManager instance;
		return instance;
	}

	// フォントが未ロードなら生成、済みならキャッシュを返す
	FontAtlas& getDefaultFont();

	// コピー・ムーブ禁止
	FontManager(const FontManager&)            = delete;
	FontManager& operator=(const FontManager&) = delete;

private:
	FontManager() = default;

	std::unique_ptr<FontAtlas> defaultFont_;

	static constexpr const char* kFontPath      = "assets/fonts/PixelMplus12-Regular.ttf";
	static constexpr float       kFontSize      = 24.0f;
	static constexpr int         kFontAtlasSize = 2048;
};

#endif // FONT_MANAGER_HPP
