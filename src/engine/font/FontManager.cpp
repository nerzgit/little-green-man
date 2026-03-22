#include "FontManager.hpp"

#include "../graphics/Texture.hpp"

FontAtlas& FontManager::getDefaultFont() {
	if (!defaultFont_) {
		defaultFont_ = std::make_unique<FontAtlas>(kFontPath, kFontSize, kFontAtlasSize);
	}
	return *defaultFont_;
}
