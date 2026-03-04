#include "Map.hpp"

#include "MapLoader.hpp"

Map::Map() : mapLoader_(std::make_unique<MapLoader>()) {
}

Map::~Map() = default;

bool Map::isWallAt(float x, float y) const {
	return mapLoader_->isWallAt(x, y);
}

bool Map::isTriggerAt(float x, float y) const {
	return mapLoader_->isTriggerAt(x, y);
}

glm::vec2 Map::getPlayerSpawnPosition() const {
	const float ts  = MapLoader::kTileSize;
	const float col = static_cast<float>(mapLoader_->getPlayerSpawnCol());
	const float row = static_cast<float>(mapLoader_->getPlayerSpawnRow());
	return { col * ts + ts / 2.0f, row * ts + ts / 2.0f };
}

float Map::getPixelWidth() const {
	return mapLoader_->getWidth() * MapLoader::kTileSize;
}

float Map::getPixelHeight() const {
	return mapLoader_->getHeight() * MapLoader::kTileSize;
}

std::vector<glm::vec2> Map::getEnemySpawnPositions() const {
	std::vector<glm::vec2> positions;
	const float            ts = MapLoader::kTileSize;
	for (const auto& [col, row] : mapLoader_->getEnemySpawns()) {
		positions.emplace_back(col * ts + ts / 2.0f, row * ts + ts / 2.0f);
	}
	return positions;
}
