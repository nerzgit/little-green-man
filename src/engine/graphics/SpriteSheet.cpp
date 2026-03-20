#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet(std::shared_ptr<Texture> texture, int frameWidth,
                         int frameHeight)
    : texture_(std::move(texture))
    , frameWidth_(frameWidth)
    , frameHeight_(frameHeight) {
}

SpriteFrame SpriteSheet::frame(int col, int row) const {
	float texW = static_cast<float>(texture_->width);
	float texH = static_cast<float>(texture_->height);
	float fw   = static_cast<float>(frameWidth_);
	float fh   = static_cast<float>(frameHeight_);

	glm::vec2 uvMin = {(col * fw) / texW, (row * fh) / texH};
	glm::vec2 uvMax = {((col + 1) * fw) / texW, ((row + 1) * fh) / texH};

	return {texture_, uvMin, uvMax};
}
