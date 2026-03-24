#include "Map.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../GameConstants.hpp"
#include "MapSpawnFinder.hpp"
#include "TileChecker.hpp"
#include <stdexcept>

Map::Map() = default;

Map::~Map() = default;

void Map::loadTileset(const std::string& tilesetPath) {
	tileset_ = std::make_unique<Tileset>(tilesetPath);
}

void Map::draw(Renderer& renderer, DrawLayer layer) {
	if (!tileset_)
		throw std::runtime_error("Tileset not loaded");
	for (const auto& l : layers_) {
		if (l.drawLayer() != layer)
			continue;
		tileset_->draw(renderer, l.loader(), GameConstants::kTileSize);
	}
}

bool Map::isTileAt(TileType type, float x, float y) const {
	for (const auto& l : layers_) {
		if (TileChecker::isTileAt(l.loader(), type, x, y))
			return true;
	}
	return false;
}

MapSize Map::getMapSize() const {
	if (layers_.empty()) return {0.0f, 0.0f};
	const float ts = GameConstants::kTileSize;
	return {
		layers_[0].loader().getWidth()  * ts,
		layers_[0].loader().getHeight() * ts,
	};
}

glm::vec2 Map::getSpawnPosition(CharacterType type) const {
	return MapSpawnFinder::spawnPosition(layers_, type);
}

std::vector<glm::vec2> Map::getSpawnPositions(CharacterType type) const {
	return MapSpawnFinder::spawnPositions(layers_, type);
}

std::unique_ptr<Map> Map::createNextMap() const {
	if (nextMapPath_.empty())
		return nullptr;
	return Map::load(nextMapPath_);
}

const std::string& Map::getStoryPath() const { return storyPath_; }
