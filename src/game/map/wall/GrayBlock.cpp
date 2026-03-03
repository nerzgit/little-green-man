#include "GrayBlock.hpp"

#include "../MapLoader.hpp"
#include "../../../engine/graphics/Renderer.hpp"

static constexpr unsigned char kRgba[4] = {120, 120, 120, 255};

GrayBlock::GrayBlock() : texture_(1, 1, kRgba) {}

void GrayBlock::draw(Renderer& renderer, glm::vec2 position) const {
	renderer.drawSprite(texture_, position, MapLoader::kTileSize,
	                    MapLoader::kTileSize);
}
