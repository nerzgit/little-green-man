#include "GrayBlock.hpp"

#include "../../../engine/graphics/Renderer.hpp"
#include "../MapLoader.hpp"

static constexpr unsigned char kRgba[4] = {120, 120, 120, 255};

GrayBlock::GrayBlock() : texture_(1, 1, kRgba) {
}

void GrayBlock::draw(Renderer& renderer, glm::vec2 position) const {
	renderer.drawSprite(texture_, position, MapLoader::kTileSize,
	                    MapLoader::kTileSize);
}
