#include "Map.hpp"
#include "MapLoader.hpp"

Map::Map() : mapLoader_(std::make_unique<MapLoader>()) {}

Map::~Map() = default;

bool Map::isWallAt(float x, float y) const {
	return mapLoader_->isWallAt(x, y);
}
