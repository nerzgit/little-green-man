#include "Map.hpp"
#include "MapLoader.hpp"

Map::Map() : mapLoader_(std::make_unique<MapLoader>()) {
}

Map::~Map() = default;

bool Map::isWallAt(float x, float y) const {
	return mapLoader_->isWallAt(x, y);
}

float Map::getPixelWidth() const {
	return mapLoader_->getWidth() * MapLoader::kTileSize;
}

float Map::getPixelHeight() const {
	return mapLoader_->getHeight() * MapLoader::kTileSize;
}
