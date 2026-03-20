#include "TilesetLoader.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../../engine/graphics/TextureCache.hpp"
#include "../GameConstants.hpp"
#include "MapLoader.hpp"

TilesetLoader::TilesetLoader(const std::string& tilesetPath)
    : tileset_(TextureCache::get(tilesetPath)) {
	tilesetCols_ = tileset_->width / GameConstants::kTilePixels;
}

void TilesetLoader::draw(Renderer& renderer, const MapLoader& mapLoader,
                         float displaySize) const {
	const float ts      = displaySize;
	const float srcSize = static_cast<float>(GameConstants::kTilePixels);
	const float texW    = static_cast<float>(tileset_->width);
	const float texH    = static_cast<float>(tileset_->height);

	for (int row = 0; row < mapLoader.getHeight(); ++row) {
		for (int col = 0; col < mapLoader.getWidth(); ++col) {
			if (mapLoader.getTileType(col, row) == TileType::WALL)
				continue;

			int idx = mapLoader.getTile(col, row).id;
			if (idx == -1)
				continue;

			int tileCol = idx % tilesetCols_;
			int tileRow = idx / tilesetCols_;

			glm::vec2 uvMin = {(tileCol * srcSize) / texW,
			                   (tileRow * srcSize) / texH};
			glm::vec2 uvMax = {((tileCol + 1) * srcSize) / texW,
			                   ((tileRow + 1) * srcSize) / texH};

			glm::vec2 center = {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
			renderer.drawTile(*tileset_, center, ts, ts, uvMin, uvMax);
		}
	}
}
